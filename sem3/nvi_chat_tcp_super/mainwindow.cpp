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
                        .arg(addressToString(adrs[id]));
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

void MainWindow::startClient()
{
    // Проверка введённного с формы порта
    bool okPort1;
    int port = ui->lineEditPort->text().toInt(&okPort1);

    if (okPort1 && port > 0 && port < 0x10000)
    {
        QString nick = getEnteredNickname();
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
                .arg(addressToString(getSocket->localAddress())));

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
            .arg(addressToString(sockets[0]->localAddress())));

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
                .arg(addressToString(getSocket->localAddress()),
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
            currentBlockSize = bytesToInt(senderSocket->read(4));
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
        case char(0): { // Если сообщение было отправлено именно сервером
            // Считывание текста
            QString text = QString::fromUtf8(data);
            logTextMessage(text, "Сервер", "3BD737");
            break;
        }
        case char(1): { // Если сообщение было отправлено кем-то из клиентов
            // Считывание никнейма
            int nickSepInd = data.indexOf(char(0));
            auto nick = QString::fromUtf8(data.left(nickSepInd));

            // Считывание текста
            data = data.mid(nickSepInd + 1);
            QString text = QString::fromUtf8(data);

            logTextMessage(text, nick, "C2145C");
            break;
        }
        case char(2): { // Если кто-то из клиентов подключился
            // Считывание IP-адреса
            auto addr = bytesToAddress(data.left(4));

            // Считывание никнейма
            data = data.mid(4);
            QString nick = QString::fromUtf8(data);

            logInfo(QString("<b>%1</b> подключился к беседе как <b>%2</b>")
                    .arg(addressToString(addr), nick));
            break;
        }
        case char(3): { // Если кто-то из клиентов отключился
            // Считывание никнейма
            auto nick = QString::fromUtf8(data);

            logWarn(QString("<b>%1</b> отключился от беседы").arg(nick));
            break;
        }
        case char(4): { // Если сервер сам прислал картинку
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
                imageFile.write(data);

                logImageMessage(pixmap, path, "Сервер", "3BD737");
            }
            else
            {
                logTextMessage("<b style=\"color:#FB5F56;font-size:12px\">Не удалось загрузить изображение</b>",
                               "Сервер", "3BD737");
            }
            break;
        }
        case char(5): { // Если клиент отправил картинку
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
                imageFile.write(data);

                logImageMessage(pixmap, path, nick, "C2145C");
            }
            else
            {
                logTextMessage("<b style=\"color:#FB5F56;font-size:12px\">Не удалось загрузить изображение</b>",
                               nick, "C2145C");
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
        case char(0): {
            // Считывание текста
            auto text = QString::fromUtf8(data);

            logTextMessage(text, userNicknames[socketId], "C2145C");

            // Создание массива байтов, отражающий данные с ником, текстом
            data.prepend(char(0));
            data.prepend(userNicknames[socketId].toUtf8());
            data.prepend(char(1));
            // Отправка данных всем сокетам
            sendData(data, socket);
            break;
        }
        case char(1): {
            // Считывание ника
            auto nick = QString::fromUtf8(data);

            userNicknames.insert(socketId, nick);

            logInfo(QString("<b>%1</b> называет себя <b>%2</b>")
                    .arg(addressToString(socket->localAddress()), nick));

            // Создания массива байтов, отражающий данные с ником
            data.prepend(addressToBytes(socket->localAddress()));
            data.prepend(char(2));
            sendData(data, socket);
            break;
        }
        case char(2): {
            // Чтение присланной картинки
            QString nick = userNicknames[socketId];

            // Считывание имени файла
            int filenameSepIndex = data.indexOf(char(0));
            QString imageName = data.left(filenameSepIndex);
            data = data.mid(filenameSepIndex + 1);

            // Получение пути к файлу
            QString path = getFilenameForDownload(imageName);
            QFile imageFile(path);

            QPixmap pixmap;
            if (pixmap.loadFromData(data) && imageFile.open(QIODevice::WriteOnly))
            {
                imageFile.write(data);

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
                logTextMessage("<b style=\"color:#FB5F56;font-size:12px\">Не удалось загрузить изображение</b>",
                               nick, "C2145C");
            }
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

void MainWindow::sendData(QByteArray data, QTcpSocket *exception)
{
    // Добавляем размер
    data.prepend(intToBytes(data.size()));
    for (auto socket : qAsConst(sockets))
    {
        if (socket != exception)
            socket->write(data);
    }
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

void MainWindow::sendImage(QString filePath)
{
    // Считываем картинку из файла
    QFile imageFile(filePath);
    int fileSize = imageFile.size();
    if (fileSize > 0 && fileSize <= 1024*1024*1024)
    {
        imageFile.open(QIODevice::ReadOnly);
        auto data = imageFile.readAll();

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
                "Он не должен быть пустым или размером больше 1ГБ");
    }
}


/// Метод для превращения адреса в красивую строку
QString MainWindow::addressToString(QHostAddress address)
{
    return address.toString().split(':').last();
}

/// Метод для превращения адреса в массив байтов
QByteArray MainWindow::addressToBytes(QHostAddress address)
{
    return intToBytes(address.toIPv4Address());
}

/// Метод для превращения массива байтов в адрес
QHostAddress MainWindow::bytesToAddress(QByteArray ba)
{
    quint32 addrInt = bytesToInt(ba);
    return QHostAddress(addrInt);
}

/// Метод для превращения числа в массив байтов
QByteArray MainWindow::intToBytes(quint32 value)
{
    char *intAsBytes = new char[4];
    memcpy(intAsBytes, &value, 4);
    QByteArray ba(intAsBytes, 4);
    delete [] intAsBytes;
    return ba;
}

/// Метод для превращения массива байтов в число
quint32 MainWindow::bytesToInt(QByteArray ba)
{
    quint32 result;
    memcpy(&result, ba.constData(), 4);
    return result;
}


/// Метод для получения адекватного ника
QString MainWindow::getEnteredNickname()
{
    return ui->lineEditNickname->text().trimmed();
}

bool MainWindow::isServerMode()
{
    return server != nullptr;
}


void MainWindow::scrollAreaRangeChanged()
{
    auto scroll = static_cast<QScrollBar *>(sender());
    if (scroll)
        scroll->setSliderPosition(ui->chatArea->verticalScrollBar()->maximum());
}


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
    mainMessageLayout->addWidget(textLabel);

    addWidgetToChat(mainContainer);

    return mainContainer;
}

QWidget *MainWindow::logImageMessage(QPixmap pixmap, QString filename, QString author, QString color)
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

    if (!filename.isEmpty())
    {
        auto showFileButton = new QPushButton("Открыть фото");
        showFileButton->setProperty("filename", filename);
        connect(showFileButton, &QPushButton::clicked, this, &MainWindow::imageOpeningButtonClicked);
        mainMessageLayout->addWidget(showFileButton);

        auto pathLabel = new QLabel(QString("<i style=\"color:#707070;font-size:8pt;\">"
                                            "Путь к файлу: <u>%1</u></i>").arg(filename));
        pathLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        pathLabel->setWordWrap(true);
        mainMessageLayout->addWidget(pathLabel);
    }

    addWidgetToChat(mainContainer);

    return mainContainer;
}

QWidget *MainWindow::logError(QString content)
{
    auto widget = genLabelWidget(content, 251, 95, 86, 30);
    addWidgetToChat(widget);

    ui->tabWidget->setCurrentIndex(1);

    return widget;
}

QWidget *MainWindow::logWarn(QString content)
{
    auto widget = genLabelWidget(content, 251, 223, 86, 30);
    addWidgetToChat(widget);

    ui->tabWidget->setCurrentIndex(1);

    return widget;
}

QWidget *MainWindow::logInfo(QString content)
{
    auto widget = genLabelWidget(content, 112, 163, 255, 30);

    addWidgetToChat(widget);

    return widget;
}

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

void MainWindow::addWidgetToChat(QWidget *widget)
{
    chatWidgets.append(widget);
    ui->verticalChatLayout->addWidget(widget);
}


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

void MainWindow::imageOpeningButtonClicked()
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

void MainWindow::showInFolder(QString filename)
{
#if defined(Q_OS_WIN)
    QStringList args;
    if (!info.isDir())
        args << "/select,";
    args << QDir::toNativeSeparators(filename);
    if (QProcess::startDetached("explorer", args))
        return;
#elif defined(Q_OS_MAC)
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
    QFileInfo info(filename);
    QDesktopServices::openUrl(QUrl::fromLocalFile(info.isDir()? filename : info.path()));
#endif
}

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

