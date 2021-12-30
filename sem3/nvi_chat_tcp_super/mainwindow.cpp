#include <QtNetwork/QNetworkDatagram>
#include <QtNetwork/QNetworkInterface>
#include <QTime>
#include <QScrollBar>
#include <QFileDialog>
#include <QDesktopServices>
#include <QProcess>
#include <QBuffer>
#include <QStandardPaths>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "byteconverter.h"

/// Конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Настройка области чата
    ui->verticalChatLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    connect(ui->chatArea->verticalScrollBar(), &QScrollBar::rangeChanged, this, &MainWindow::scrollAreaRangeChanged);

    // Палитра для полей и кнопки
    normPal = ui->lineEditPort->palette();
    errPal = normPal;
    errPal.setColor(QPalette::Text, QColor(248, 23, 62));
    errPal.setColor(QPalette::ButtonText, QColor(248, 23, 62));

    // По нажатию Enter в поле ввода сообщения, сообщение должно отправляться
    connect(ui->lineEditMessage, &QLineEdit::returnPressed, this, &MainWindow::on_pushButtonSend_clicked);

    ui->lineEditMessage->setEnabled(false);
    ui->pushButtonSend->setEnabled(false);
    ui->checkBoxSpam->setChecked(false);
    ui->attachmentsPage->setEnabled(false);

    // Запуск таймера для спама в чат
    spamTimer.setInterval(500);
    spamTimer.start();
    connect(&spamTimer, &QTimer::timeout, this, &MainWindow::onSpam);

    ui->ServerWidget->hide();
    ui->ClientWidget->hide();

    ui->propertiesTabLayout->setAlignment(Qt::AlignLeft);
    on_pushButtonAddresses_clicked();
}

/// Деструктор
MainWindow::~MainWindow()
{
    clearServer();
    clearSockets();

    delete ui;
}


/// Вывод айпи адресов компьютера
void MainWindow::on_pushButtonAddresses_clicked()
{
    // Получаем все адреса
    auto adrs = QNetworkInterface::allAddresses();
    if (adrs.length() > 0)
    {
        QString str = "IP адреса компьютера:";
        int id = 1;
        for (int i = 0; i < adrs.count(); i++)
        {
            // Отсеиваем только адреса типа IPv4
            if (adrs[i].protocol() == QAbstractSocket::IPv4Protocol)
            {
                str += QString("<br>%1) <b>%2</b>").arg(id)
                        .arg(ByteConverter::addressToString(adrs[i]));
                id++;
            }
        }
        ui->labelAddresses->setText(str);
    }
    else
    {
        ui->labelAddresses->setText("К этому комьютеру не относится ни один IP адрес");
    }
}


/// Выбор режима сервера
void MainWindow::on_radioButtonServer_clicked()
{
    ui->ServerWidget->show();
    ui->ClientWidget->hide();
}

/// Выбор режима клиента
void MainWindow::on_radioButtonClient_clicked()
{
    ui->ServerWidget->hide();
    ui->ClientWidget->show();
}


/// Отправка введённого сообщения
void MainWindow::on_pushButtonSend_clicked()
{
    sendMessage(ui->lineEditMessage->text());

    ui->lineEditMessage->clear();
    ui->lineEditMessage->setFocus();
}

/// Отправка выбранного изображения
void MainWindow::on_pushButtonSendImage_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Открыть файл",
                                                    "", tr("Images (*.png *.jpg *.gif)"));
    if (!filename.isEmpty())
    {
        sendImage(filename);
    }
    else
    {
        logWarn("Файл не выбран");
    }
}

/// Отправка выбранного файла
void MainWindow::on_pushButtonSendFile_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Открыть файл",
                                                    "");
    if (!filename.isEmpty())
    {
        sendFile(filename);
    }
    else
    {
        logWarn("Файл не выбран");
    }
}


/// Слот открытия порта сервера
void MainWindow::on_pushButtonBind_clicked()
{
    // Проверка введённного с формы порта
    if (server == nullptr)
    {
        startServer();
    }
    else
    {
        stopServer();
    }
}

/// Метод для запуска сервера
void MainWindow::startServer()
{
    bool okPort2;
    int localPort = ui->lineEditLocalPort->text().toInt(&okPort2);

    if (okPort2 && localPort > 0 && localPort < 0x10000)
    {
        ui->lineEditLocalPort->setPalette(normPal);
        ui->pushButtonBind->setPalette(normPal);

        // Для начала на всякий слйчай отключаемся от сокета, сервера
        clearSockets();
        clearServer();

        // Создаём TCP сервер и открываем порт
        server = new QTcpServer(this);
        connect(server, &QTcpServer::newConnection, this, &MainWindow::onTcpBinded);
        if (server->listen(QHostAddress::Any, quint16(localPort)))
        {
            logInfo("Прослушиваем порт <b>" + QString::number(localPort) + "</b>");

            ui->pushButtonBind->setText("Закрыть");
            ui->lineEditLocalPort->setEnabled(false);

            ui->tabWidget->setCurrentIndex(1);
        }
        else
        {
            logError("Ошибка при открытии сервера");
        }
    }
    else
    {
        // Если порт неправильный, сообщаем об ошибке
        ui->pushButtonBind->setPalette(errPal);
        ui->lineEditLocalPort->setPalette(errPal);
        logError("Введён неправильный порт");
    }
}

/// Метод для остановки сервера
void MainWindow::stopServer()
{
    logInfo("Сервер остановлен");
    clearSockets();
    clearServer();
}

/// Слот установки соединения
void MainWindow::on_pushButtonConnect_clicked()
{
    if (sockets.length() == 0 || isServerMode())
    {
        startClient();
    }
    else
    {
        stopClient();
    }
}

/// Метод для запуска клиента
void MainWindow::startClient()
{
    // Проверка введённного с формы порта
    bool okPort1;
    int port = ui->lineEditPort->text().toInt(&okPort1);

    if (okPort1 && port > 0 && port < 0x10000)
    {
        QString nick = ui->lineEditNickname->text().trimmed();
        if (nick.size() > 0)
        {
            userConnectionNick = nick;

            ui->lineEditPort->setPalette(normPal);
            ui->pushButtonConnect->setPalette(normPal);

            // Для начала на всякий слйчай отключаемся от сокета, сервера
            clearSockets();
            clearServer();

            // Создаём принимающий TCP сокет
            QTcpSocket *sendSocket = new QTcpSocket(this);
            connect(sendSocket, &QTcpSocket::connected, this, &MainWindow::onTcpConnect);
            connect(sendSocket, &QTcpSocket::disconnected, this, &MainWindow::onTcpSendDisconnect);
            connect(sendSocket, &QTcpSocket::readyRead, this, &MainWindow::onTcpReadyRead);
            // В версии Qt 5.15 добавили сигнал errorOccured, который является заменой ныне устаревшего сигнала error
#if QT_VERSION_MAJOR >= 5 && QT_VERSION_MINOR >= 15
            connect(sendSocket, &QTcpSocket::errorOccurred, this, &MainWindow::onTcpSendError);
#else
            connect(sendSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MainWindow::onTcpSendError);
#endif
            // Подключаемся к серверу
            sendSocket->connectToHost(ui->lineEditAddress->text(), quint16(port));

            // Добавляем новый сокет
            sockets.append(sendSocket);

            ui->pushButtonConnect->setText("Отключиться");
            ui->lineEditPort->setEnabled(false);
            ui->lineEditAddress->setEnabled(false);
            ui->lineEditNickname->setEnabled(false);

            ui->tabWidget->setCurrentIndex(1);
        }
        else
        {
            // Если порт неправильный, сообщаем об ошибке
            ui->pushButtonConnect->setPalette(errPal);
            ui->lineEditNickname->setPalette(errPal);
            logError("Введён неправильный никнейм");
        }
    }
    else
    {
        // Если порт неправильный, сообщаем об ошибке
        ui->pushButtonConnect->setPalette(errPal);
        ui->lineEditPort->setPalette(errPal);
        logError("Введён неправильный порт");
    }
}

/// Метод для остановки клиента
void MainWindow::stopClient()
{
    clearSockets();
    clearServer();
}


/// Слот, соединённый с сигналом сервера, создаёт новое принимающее соединение
void MainWindow::onTcpBinded()
{
    QTcpSocket *getSocket = server->nextPendingConnection();
    if (getSocket != nullptr)
    {
        connect(getSocket, &QTcpSocket::disconnected, this, &MainWindow::onTcpGetDisconnect);
        connect(getSocket, &QTcpSocket::readyRead, this, &MainWindow::onTcpReadyRead);

        logInfo(QString("К серверу подключился <b>%1</b>!")
                .arg(ByteConverter::addressToString(getSocket->localAddress())));

        // Добавляем новый сокет
        sockets.append(getSocket);

        ui->lineEditMessage->setEnabled(true);
        ui->pushButtonSend->setEnabled(true);
        ui->spamPage->setEnabled(true);
        ui->attachmentsPage->setEnabled(true);
    }
}

/// Слот, соединенённый с сигналом удачного соединения отправляющего сокета
void MainWindow::onTcpConnect()
{
    // Отправка ника серверу
    QByteArray ba = userConnectionNick.toUtf8();
    ba.prepend(char(1));
    sendData(ba);

    logInfo(QString("Соединение с <b>%1</b> установлено!")
            .arg(ByteConverter::addressToString(sockets[0]->localAddress())));

    ui->lineEditMessage->setEnabled(true);
    ui->pushButtonSend->setEnabled(true);
    ui->spamPage->setEnabled(true);
    ui->attachmentsPage->setEnabled(true);
}

/// Слот, соединённый с сигналом дисконнекта отправляющего сокета
void MainWindow::onTcpSendDisconnect()
{
    logWarn("Вы отключились от сервера");

    ui->lineEditMessage->setEnabled(false);
    ui->pushButtonSend->setEnabled(false);
    ui->spamPage->setEnabled(false);
    ui->checkBoxSpam->setChecked(false);
    ui->attachmentsPage->setEnabled(false);

    if (!isClearing)
    {
        clearSockets();
    }
}

/// Слот, соединённый с сигналом дисконнекта принимающего сокета
void MainWindow::onTcpGetDisconnect()
{
    auto getSocket = static_cast<QTcpSocket *>(sender());
    if (getSocket != nullptr)
    {
        int socketId = sockets.indexOf(getSocket);
        logWarn(QString("Собеседник <b>%1</b>(<b>%2</b>) отключился от вас")
                .arg(ByteConverter::addressToString(getSocket->localAddress()),
                     userNicknames[socketId]));

        int userId = sockets.indexOf(getSocket);
        auto nick = userNicknames[userId];

        auto disconnectData = nick.toUtf8();
        disconnectData.prepend(char(3));
        sendData(disconnectData, getSocket);

        // Удаляем сокет из вектора сокетов
        if (!isClearing)
        {
            sockets.removeOne(getSocket);
            getSocket->deleteLater();
            userNicknames.removeAt(userId);
        }

        if (sockets.length() == 0)
        {
            // Если больше нет активных сокетов, отключаем возможность отправлять сообщения
            ui->lineEditMessage->setEnabled(false);
            ui->pushButtonSend->setEnabled(false);
            ui->spamPage->setEnabled(false);
            ui->checkBoxSpam->setChecked(false);
            ui->attachmentsPage->setEnabled(false);
        }
    }
}

/// Слот, соединённый с сигналом об ошибки отправляющего сокета
void MainWindow::onTcpSendError(QAbstractSocket::SocketError err)
{
    // Небольшая обработка "известных" ошибок сокета,
    // чтобы не вводить пользователя в заблуждение
    switch (err) {
    case QAbstractSocket::RemoteHostClosedError:
        logError("Соединение с сервером закрылось");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        logError("Соединение отклонено");
        break;
    case QAbstractSocket::HostNotFoundError:
        logError("Хост не найден");
        break;
    default:
        logError("Ошибка сокета отправки");
    }

    clearSockets();

    ui->lineEditMessage->setEnabled(false);
    ui->pushButtonSend->setEnabled(false);
    ui->spamPage->setEnabled(false);
    ui->checkBoxSpam->setChecked(false);
    ui->attachmentsPage->setEnabled(false);
}

/// Слот, соединённый с сигналом приёма данных принимающего сокета
void MainWindow::onTcpReadyRead()
{
    auto senderSocket = static_cast<QTcpSocket *>(sender());
    if (senderSocket != nullptr)
    {
        processPendingData(senderSocket);
    }
}

/// Метод для обработки данных из сокета
void MainWindow::processPendingData(QTcpSocket *senderSocket)
{
    // Считывание ожидаемого количества байтов
    int expectedSize = senderSocket->bytesAvailable();
    if (expectedSize > 0)
    {
        // Если ещё не известен размер блока, то считываем его
        if (currentBlockSize == -1 && expectedSize >= 4)
        {
            currentBlockSize = ByteConverter::bytesToInt(senderSocket->read(4));
            expectedSize -= 4;
        }

        if (expectedSize >= currentBlockSize)
        {
            // Загружаем столько данных, сколько надо
            QByteArray data = senderSocket->read(currentBlockSize);

            // Обрабатываем загруженные данные
            if (isServerMode())
            {
                // Если сервер получает данные
                processClientData(data, senderSocket);
            }
            else
            {
                // Если клиент получает данные
                processServerData(data, senderSocket);
            }

            currentBlockSize = -1;
            processPendingData(senderSocket);
        }
    }
}

/// Метод для обработки данных, приходящих от сервера
void MainWindow::processServerData(QByteArray data, QTcpSocket *)
{
    if (!data.isEmpty())
    {
        char command = data[0];
        data = data.mid(1);
        switch (command) {
        // Сообщение от сервера
        case char(0): {
            // Считывание текста
            QString text = QString::fromUtf8(data);
            logTextMessage(text, "Сервер", "3BD737");
            break;
        }

            // Сообщение от клиента
        case char(1): {
            // Считывание никнейма
            int nickSepInd = data.indexOf(char(0));
            auto nick = QString::fromUtf8(data.left(nickSepInd));

            // Считывание текста
            data = data.mid(nickSepInd + 1);
            QString text = QString::fromUtf8(data);

            logTextMessage(text, nick, "C2145C");
            break;
        }

            // Уведомление об подключении клиента
        case char(2): {
            // Считывание IP-адреса
            auto addr = ByteConverter::bytesToAddress(data.left(4));

            // Считывание никнейма
            data = data.mid(4);
            QString nick = QString::fromUtf8(data);

            logInfo(QString("<b>%1</b> подключился к беседе как <b>%2</b>")
                    .arg(ByteConverter::addressToString(addr), nick));
            break;
        }

            // Уведомление об отключении клиента
        case char(3): {
            // Считывание никнейма
            auto nick = QString::fromUtf8(data);

            logWarn(QString("<b>%1</b> отключился от беседы").arg(nick));
            break;
        }

            // Картинка от сервера
        case char(4): {
            // Считывание имени картинки
            int filenameSepIndex = data.indexOf(char(0));
            QString imageName = data.left(filenameSepIndex);
            data = data.mid(filenameSepIndex + 1);

            // Получение пути к файлу
            QString path = getFilenameForDownload(imageName);
            QFile imageFile(path);

            // Сохранение и вывод картинку
            QPixmap pixmap;
            if (pixmap.loadFromData(data) && imageFile.open(QIODevice::WriteOnly))
            {
                // Сохранение файла
                imageFile.write(data);

                logImageMessage(pixmap, path, "Сервер", "3BD737");
            }
            else
            {
                logError("Не удалось загрузить изображение");
            }
            break;
        }

            // Картинка от клиента
        case char(5): {
            // Считывание никнейма
            int nickSepInd = data.indexOf(char(0));
            QString nick = QString::fromUtf8(data.left(nickSepInd));
            data = data.mid(nickSepInd + 1);

            // Считывание имени картинки
            int filenameSepIndex = data.indexOf(char(0));
            QString imageName = data.left(filenameSepIndex);
            data = data.mid(filenameSepIndex + 1);

            // Получение пути к файлу
            QString path = getFilenameForDownload(imageName);
            QFile imageFile(path);

            // Сохранение и вывод картинку
            QPixmap pixmap;
            if (pixmap.loadFromData(data) && imageFile.open(QIODevice::WriteOnly))
            {
                // Сохранение файла
                imageFile.write(data);

                logImageMessage(pixmap, path, nick, "C2145C");
            }
            else
            {
                logError("Не удалось загрузить изображение");
            }
            break;
        }

            // Файл от сервера
        case char(6): {
            // Считывание имени файла
            int filenameSepIndex = data.indexOf(char(0));
            QString fileName = data.left(filenameSepIndex);
            data = data.mid(filenameSepIndex + 1);

            // Получение пути к файлу
            QString path = getFilenameForDownload(fileName);
            QFile file(path);
            if (file.open(QIODevice::WriteOnly))
            {
                // Сохранение файла
                file.write(data);

                logFileMessage(path, "Сервер", "3BD737");
            }
            else
            {
                logError("Не удалось сохранить файл");
            }
            break;
        }

            // Файл от клиента
        case char(7): {
            // Считывание никнейма
            int nickSepInd = data.indexOf(char(0));
            QString nick = QString::fromUtf8(data.left(nickSepInd));
            data = data.mid(nickSepInd + 1);

            // Считывание имени файла
            int filenameSepIndex = data.indexOf(char(0));
            QString fileName = data.left(filenameSepIndex);
            data = data.mid(filenameSepIndex + 1);

            // Получение пути к файлу
            QString path = getFilenameForDownload(fileName);
            QFile file(path);
            if (file.open(QIODevice::WriteOnly))
            {
                // Сохранение файла
                file.write(data);

                logFileMessage(path, nick, "C2145C");
            }
            else
            {
                logError("Не удалось сохранить файл");
            }
            break;
        }
        }
    }
}

/// Метод для обработки данных, приходящих от клиентов
void MainWindow::processClientData(QByteArray data, QTcpSocket *socket)
{
    if (!data.isEmpty())
    {
        int socketId = sockets.indexOf(socket);

        char command = data[0];
        data = data.mid(1);
        switch (command) {
        // Сообщение
        case char(0): {
            // Считывание текста
            auto text = QString::fromUtf8(data);

            logTextMessage(text, userNicknames[socketId], "C2145C");

            // Пересылка сообщения всем другим пользователям
            data.prepend(char(0));
            data.prepend(userNicknames[socketId].toUtf8());
            data.prepend(char(1));
            sendData(data, socket);
            break;
        }

            // Блок с ником пользователя
        case char(1): {
            // Считывание ника
            auto nick = QString::fromUtf8(data);

            userNicknames.insert(socketId, nick);

            logInfo(QString("<b>%1</b> называет себя <b>%2</b>")
                    .arg(ByteConverter::addressToString(socket->localAddress()), nick));

            // Создания массива байтов, отражающий данные с ником
            data.prepend(ByteConverter::addressToBytes(socket->localAddress()));
            data.prepend(char(2));
            sendData(data, socket);
            break;
        }

            // Картинка
        case char(2): {
            // Считывание ника
            QString nick = userNicknames[socketId];

            // Считывание имени файла
            int filenameSepIndex = data.indexOf(char(0));
            QString imageName = QString::fromUtf8(data.left(filenameSepIndex));
            data = data.mid(filenameSepIndex + 1);

            // Получение пути к файлу
            QString path = getFilenameForDownload(imageName);
            QFile imageFile(path);

            QPixmap pixmap;
            if (pixmap.loadFromData(data) && imageFile.open(QIODevice::WriteOnly))
            {
                imageFile.write(data);
                imageFile.close();

                // Пересылка сообщения всем другим пользователям
                data.prepend(char(0));
                data.prepend(imageName.toUtf8());
                data.prepend(char(0));
                data.prepend(nick.toUtf8());
                data.prepend(char(5));
                sendData(data, socket);

                logImageMessage(pixmap, path, nick, "C2145C");
            }
            else
            {
                logError("Не удалось загрузить изображение");
            }
            break;
        }

            // Файл (сами внутренности файла)
        case char(3): {
            // Считывание ника
            QString nick = userNicknames[socketId];

            // Считывание имени файла
            int filenameSepIndex = data.indexOf(char(0));
            QString fileName = QString::fromUtf8(data.left(filenameSepIndex));
            data = data.mid(filenameSepIndex + 1);

            // Получение пути к файлу
            QString path = getFilenameForDownload(fileName);
            QFile file(path);

            // Запись файла
            if (file.open(QIODevice::WriteOnly))
            {
                file.write(data);
                file.close();

                // Пересылка сообщения всем другим пользователям
                data.prepend(char(0));
                data.prepend(fileName.toUtf8());
                data.prepend(char(0));
                data.prepend(nick.toUtf8());
                data.prepend(char(7));
                sendData(data, socket);

                logFileMessage(path, nick, "C2145C");
            }
            else
            {
                logError("Не удалось сохранить файл");
            }

            break;
        }
        }
    }
}


/// Метод для очистки сокетов
void MainWindow::clearSockets()
{
    isClearing = true;

    int length = sockets.length();
    for (int i = 0; i < length; i++)
    {
        auto socket = sockets[i];
        // Если сокет активный, то отключаемся от него
        if (socket->isOpen())
        {
            socket->close();
            socket->disconnect();
        }
        // Отправляем на удаление
        socket->deleteLater();
    }
    // Очищаем массив указателей на сокеты
    sockets.clear();
    userNicknames.clear();

    isClearing = false;

    ui->pushButtonConnect->setText("Подключиться");
    ui->lineEditPort->setEnabled(true);
    ui->lineEditAddress->setEnabled(true);
    ui->lineEditNickname->setEnabled(true);
}

/// Метод для остановки о удаления сервера
void MainWindow::clearServer()
{
    if (server != nullptr)
    {
        // Отключаем сервер, если он активный
        if (server->isListening())
            server->close();
        // Отправляем на удаление
        server->deleteLater();
        server = nullptr;
    }
    ui->pushButtonBind->setText("Создать");
    ui->lineEditLocalPort->setEnabled(true);
}

/// Универсальный метод для отправки данных всем нужным сокетам
void MainWindow::sendData(QByteArray data, QTcpSocket *exception)
{
    // Добавляем размер
    data.prepend(ByteConverter::intToBytes(data.size()));
    for (auto socket : qAsConst(sockets))
    {
        if (socket != exception)
            socket->write(data);
    }
}

/// Универсальный метод для отправки данных конкртеному сокету
void MainWindow::sendDataTo(QByteArray data, QTcpSocket *target)
{
    // Добавляем размер
    data.prepend(ByteConverter::intToBytes(data.size()));
    target->write(data);
}


/// Метод для более удобной отправки сообщений
void MainWindow::sendMessage(QString message)
{
    if (message.length() > 0)
    {
        // Отправка данных
        QByteArray data = message.toUtf8();
        data.prepend(char(0));
        sendData(data);

        // Вывод сообщения в "консоль"
        logTextMessage(message, "Вы", "8314C2");
    }
}

/// Метод для более удобной отправки изображений
void MainWindow::sendImage(QString filePath)
{
    // Считываем картинку из файла
    QFile imageFile(filePath);
    int fileSize = imageFile.size();
    if (fileSize > 0 && fileSize <= 256*1024*1024)
    {
        imageFile.open(QIODevice::ReadOnly);
        auto data = imageFile.readAll();
        imageFile.close();

        QPixmap pixmap;
        if (pixmap.loadFromData(data))
        {
            data.prepend(char(0));
            // Добавляем имя файла в массив байтов
            data.prepend(QFileInfo(imageFile).fileName().toUtf8());
            // Добавляем "команду". Если сервер - 4, если клиент - 2
            if (isServerMode())
                data.prepend(char(4));
            else
                data.prepend(char(2));
            sendData(data);

            logImageMessage(pixmap, filePath, "Вы", "8314C2");
        }
        else
        {
            logError("Не удалось загрузить изображение");
        }
    }
    else
    {
        logWarn("Файл слишком большой или слишком маленький\n"
                "Он не должен быть пустым или размером больше 256МБ");
    }
}

/// Метод для более удобной отправки файлов
void MainWindow::sendFile(QString filePath)
{
    // Считываем из файла
    QFile file(filePath);
    int fileSize = file.size();
    if (fileSize > 0 && fileSize <= 1024*1024*1024)
    {
        if (file.open(QIODevice::ReadOnly))
        {
            auto data = file.readAll();
            file.close();

            data.prepend(char(0));
            // Добавляем имя файла в массив байтов
            data.prepend(QFileInfo(file).fileName().toUtf8());
            // Добавляем "команду". Если сервер - 6, если клиент - 3
            if (isServerMode())
                data.prepend(char(6));
            else
                data.prepend(char(3));
            sendData(data);

            logFileMessage(filePath, "Вы", "8314C2");
        }
        else
        {
            logError("Не удалось открыть файл");
        }
    }
    else
    {
        logWarn("Файл слишком большой или слишком маленький\n"
                "Он не должен быть пустым или размером больше 1ГБ");
    }
}


/// Проверка на то, какой режим активен, true - если сервер, false - если нет
bool MainWindow::isServerMode()
{
    return server != nullptr;
}


/// Слот для перемещения скролла в низ
void MainWindow::scrollAreaRangeChanged()
{
    auto scroll = static_cast<QScrollBar *>(sender());
    if (scroll != nullptr)
        scroll->setSliderPosition(ui->chatArea->verticalScrollBar()->maximum());
}


/// Метод для вывода текстового сообщения в чат
QWidget *MainWindow::logTextMessage(QString content, QString author, QString color)
{
    auto mainContainer = new QWidget;
    auto mainMessageLayout = new QVBoxLayout;
    mainContainer->setLayout(mainMessageLayout);

    auto topInfoContainer = new QWidget;
    auto topInfoLayout = new QHBoxLayout;
    topInfoLayout->setContentsMargins(0, 0, 0, 0);
    topInfoContainer->setLayout(topInfoLayout);
    mainMessageLayout->addWidget(topInfoContainer);

    auto authorLabel = new QLabel(QString("[<b style=\"color:#%1\">%2</b>]")
                                  .arg(color, author));
    authorLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    QString ctStr = QTime::currentTime().toString("hh:mm:ss");
    auto timeLabel = new QLabel(QString("<i style=\"color:#707070;font-size:8pt;\">%2</i>")
                                .arg(ctStr));
    timeLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    topInfoLayout->addWidget(authorLabel);
    topInfoLayout->addItem(new QSpacerItem(0, 5, QSizePolicy::Expanding, QSizePolicy::Fixed));
    topInfoLayout->addWidget(timeLabel);

    auto textLabel = new QLabel(content);
    textLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    textLabel->setWordWrap(true);
    textLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    mainMessageLayout->addWidget(textLabel);

    addWidgetToChat(mainContainer);

    return mainContainer;
}

/// Метод для вывода сообщения с картинкой в чат
QWidget *MainWindow::logImageMessage(QPixmap pixmap, QString filePath, QString author, QString color)
{
    auto mainContainer = new QWidget;
    auto mainMessageLayout = new QVBoxLayout;
    mainContainer->setLayout(mainMessageLayout);

    auto topInfoContainer = new QWidget;
    auto topInfoLayout = new QHBoxLayout;
    topInfoLayout->setContentsMargins(0, 0, 0, 0);
    topInfoContainer->setLayout(topInfoLayout);
    mainMessageLayout->addWidget(topInfoContainer);

    auto authorLabel = new QLabel(QString("[<b style=\"color:#%1\">%2</b>]")
                                  .arg(color, author));
    authorLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    QString ctStr = QTime::currentTime().toString("hh:mm:ss");
    auto timeLabel = new QLabel(QString("<i style=\"color:#707070;font-size:8pt;\">%1</i>")
                                .arg(ctStr));
    timeLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    topInfoLayout->addWidget(authorLabel);
    topInfoLayout->addItem(new QSpacerItem(0, 5, QSizePolicy::Expanding, QSizePolicy::Fixed));
    topInfoLayout->addWidget(timeLabel);

    auto imageLabel = new QLabel;
    pixmap = pixmap.scaledToWidth(290);
    imageLabel->setPixmap(pixmap);
    mainMessageLayout->addWidget(imageLabel);

    if (!filePath.isEmpty())
    {
        auto showFileButton = new QPushButton("Открыть фото");
        showFileButton->setProperty("filename", filePath);
        connect(showFileButton, &QPushButton::clicked, this, &MainWindow::openFileButtonClicked);
        mainMessageLayout->addWidget(showFileButton);

        auto pathLabel = new QLabel(QString("<i style=\"color:#707070;font-size:8pt;\">"
                                            "Путь к файлу: <u>%1</u></i>").arg(filePath));
        pathLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        pathLabel->setWordWrap(true);
        mainMessageLayout->addWidget(pathLabel);
    }

    addWidgetToChat(mainContainer);

    return mainContainer;
}

/// Метод для вывода сообщения с файлов в чат
QWidget *MainWindow::logFileMessage(QString filePath, QString author, QString color)
{
    auto mainContainer = new QWidget;
    auto mainMessageLayout = new QVBoxLayout;
    mainContainer->setLayout(mainMessageLayout);

    auto topInfoContainer = new QWidget;
    auto topInfoLayout = new QHBoxLayout;
    topInfoLayout->setContentsMargins(0, 0, 0, 0);
    topInfoContainer->setLayout(topInfoLayout);
    mainMessageLayout->addWidget(topInfoContainer);

    auto authorLabel = new QLabel(QString("[<b style=\"color:#%1\">%2</b>]")
                                  .arg(color, author));
    authorLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    QString ctStr = QTime::currentTime().toString("hh:mm:ss");
    auto timeLabel = new QLabel(QString("<i style=\"color:#707070;font-size:8pt;\">%2</i>")
                                .arg(ctStr));
    timeLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    topInfoLayout->addWidget(authorLabel);
    topInfoLayout->addItem(new QSpacerItem(0, 5, QSizePolicy::Expanding, QSizePolicy::Fixed));
    topInfoLayout->addWidget(timeLabel);

    auto pathLabel = new QLabel(QString("<i style=\"color:#707070;font-size:10pt;\">"
                                        "Был отправлен файл: <u>%1</u></i>").arg(filePath));
    pathLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    pathLabel->setWordWrap(true);
    mainMessageLayout->addWidget(pathLabel);

    auto showFileButton = new QPushButton("Показать файл");
    showFileButton->setProperty("filename", filePath);
    connect(showFileButton, &QPushButton::clicked, this, &MainWindow::showFileButtonClicked);
    mainMessageLayout->addWidget(showFileButton);

    addWidgetToChat(mainContainer);

    return mainContainer;
}

/// Метод для вывода ошибки в чат
QWidget *MainWindow::logError(QString content)
{
    auto widget = genLabelWidget(content, 251, 95, 86, 30);
    addWidgetToChat(widget);

    ui->tabWidget->setCurrentIndex(1);

    return widget;
}

/// Метод для вывода предупреждения в чат
QWidget *MainWindow::logWarn(QString content)
{
    auto widget = genLabelWidget(content, 251, 223, 86, 30);
    addWidgetToChat(widget);

    ui->tabWidget->setCurrentIndex(1);

    return widget;
}

/// Метод для вывода информации в чат
QWidget *MainWindow::logInfo(QString content)
{
    auto widget = genLabelWidget(content, 112, 163, 255, 30);

    addWidgetToChat(widget);

    return widget;
}

/// Метод для генерации виджетов информационных "круглых" сообщений
QWidget *MainWindow::genLabelWidget(QString content, int r, int g, int b, int a)
{
    QLabel *label = new QLabel(content);
    label->setMinimumHeight(30);
    label->setWordWrap(true);
    label->setTextInteractionFlags(Qt::TextSelectableByMouse);
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    label->setStyleSheet(QString("background-color:rgba(%1,%2,%3,%4);border-radius:15px;")
                         .arg(r).arg(g).arg(b).arg(a));
    return label;
}

/// Метод для добавления виджета в чат
void MainWindow::addWidgetToChat(QWidget *widget)
{
    chatWidgets.append(widget);
    ui->verticalChatLayout->addWidget(widget);
}

/// Метод для удаления виджета из чата
void MainWindow::removeWidgetFromChat(QWidget *widget)
{
    chatWidgets.removeOne(widget);
    ui->verticalChatLayout->removeWidget(widget);
}


/// Метод для получения имени файла в папке для загрузки на компьютере
QString MainWindow::getFilenameForDownload(QString name)
{
    auto downloads = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    if (!downloads.isEmpty())
    {
        QDir dir(downloads);
        dir.mkdir("SuperChat");
        downloads += "/SuperChat/" + name;
        return downloads;
    }
    else
    {
        return "";
    }
}


/// Слот для открытия файла (вызывается нажатием кнопки в чате)
void MainWindow::openFileButtonClicked()
{
    auto buttonSender = static_cast<QPushButton *>(sender());
    if (buttonSender != nullptr)
    {
        auto filename = buttonSender->property("filename").value<QString>();
        if (filename != "")
        {
            openFile(filename);
        }
    }
}

/// Слот для отображения файла в проводнике (вызывается нажатием кнопки в чате)
void MainWindow::showFileButtonClicked()
{
    auto buttonSender = static_cast<QPushButton *>(sender());
    if (buttonSender != nullptr)
    {
        auto filename = buttonSender->property("filename").value<QString>();
        if (filename != "")
        {
            showInFolder(filename);
        }
    }
}


/// Метод для показывания файла в проводнике
void MainWindow::showInFolder(QString filename)
{
#if defined(Q_OS_WIN)
    // Если программа собирается под Windowss
    QStringList args;
    if (!QFileInfo(filename).isDir())
        args << "/select,";
    args << QDir::toNativeSeparators(filename);
    if (QProcess::startDetached("explorer", args))
        return;
#elif defined(Q_OS_MAC)
    // Если программа собирается под MacOS
    QStringList args;
    args << "-e";
    args << "tell application \"Finder\"";
    args << "-e";
    args << "activate";
    args << "-e";
    args << "select POSIX file \"" + filename + "\"";
    args << "-e";
    args << "end tell";
    args << "-e";
    args << "return";
    if (!QProcess::execute("/usr/bin/osascript", args))
        return;
#else
    // Если программа выполняется под другой ОС (например Linux)
    QFileInfo info(filename);
    QDesktopServices::openUrl(QUrl::fromLocalFile(info.isDir()? filename : info.path()));
#endif
}

/// Метод для открытия файла
void MainWindow::openFile(QString filename)
{
    QDesktopServices::openUrl(filename);
}


/// Слот автоматической отправки сообщения
void MainWindow::onSpam()
{
    QString str = ui->lineEditSpamText->text();
    if (ui->checkBoxSpam->isChecked() &&
            ui->lineEditMessage->isEnabled() && !str.isEmpty())
    {
        // Вывод сообщение о спаме
        sendMessage(str);
    }
}

/// Слот очистки чата
void MainWindow::on_pushButtonClear_clicked()
{
    for (int i = 0; i < chatWidgets.length(); i++)
    {
        ui->verticalChatLayout->removeWidget(chatWidgets[i]);
        chatWidgets[i]->deleteLater();
    }
    chatWidgets.clear();
}

/// Слот изменения интервала таймера
void MainWindow::on_timerIntervalSpinBox_valueChanged(double arg1)
{
    spamTimer.setInterval(arg1 * 1000);
}
