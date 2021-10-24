#include <QtNetwork/QNetworkDatagram>
#include <QtNetwork/QNetworkInterface>
#include <QTime>
#include <QTextCodec>

#include "mainwindow.h"
#include "ui_mainwindow.h"

/// Конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Палитра для полей и кнопки
    normPal = ui->lineEditPort->palette();
    errPal = normPal;
    errPal.setColor(QPalette::Text, QColor(248, 23, 62));
    errPal.setColor(QPalette::ButtonText, QColor(248, 23, 62));

    // По нажатию Enter в поле ввода сообщения, сообщение должно отправляться
    connect(ui->lineEditMessage, &QLineEdit::returnPressed, this, &MainWindow::on_pushButtonSend_clicked);

    ui->lineEditMessage->setEnabled(false);
    ui->pushButtonSend->setEnabled(false);
    ui->checkBoxResend->setEnabled(false);
    ui->pushButtonConnectionList->setEnabled(false);

    // Получение адреса текущего ПК
    auto adrs = QNetworkInterface::allAddresses();
    // В Windows (на компьютерах в Б217) выводится список с несколько другими адресами и там нужный адрес под индексом 1
    // В Linux (на личном компьютере) в списке нужный адрес находится под индексом 2
    // Также добавляю проверку на количество адресов, так как если устройство не подключено ни к какой сети,
    // то там всего 1-2 элеметов (localhost собственно)
#ifdef Q_OS_WIN
    if (adrs.length() >= 2)
    {
        QString adr = adrs[1].toString();
#else
    if (adrs.length() >= 3)
    {
        QString adr = adrs[2].toString();
#endif
        ui->labelLocalAddress->setText("Адрес компьютера: <b>" + adr + "</b>");
    }

    // Запуск таймера для спама в чат
    spamTimer.setInterval(500);
    spamTimer.start();
    connect(&spamTimer, &QTimer::timeout, this, &MainWindow::onSpam);

    ui->ServerWidget->hide();
    ui->ClientWidget->hide();
}

/// Деструктор
MainWindow::~MainWindow()
{
    delete ui;
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
    sendMessage(ui->lineEditMessage->text(), "Вы", "8314C2");
}

/// Слот открытия порта сервера
void MainWindow::on_pushButtonBind_clicked()
{
    // Проверка введённного с формы порта
    bool okPort2;
    quint16 localPort = quint16(ui->lineEditLocalPort->text().toInt(&okPort2));
    if (okPort2)
        ui->lineEditLocalPort->setPalette(normPal);
    else
        ui->lineEditLocalPort->setPalette(errPal);

    if (okPort2)
    {
        ui->pushButtonConnect->setPalette(normPal);

        // Для начала на всякий слйчай отключаемся от сокета, сервера
        clearSockets();
        clearServer();

        // Создаём TCP сервер и открываем порт
        server = new QTcpServer(this);
        connect(server, &QTcpServer::newConnection, this, &MainWindow::onTcpBinded);
        if (server->listen(QHostAddress::Any, localPort))
        {
            logInfo("Прослушиваем порт <b>" + QString::number(localPort) + "</b>");

            ui->checkBoxResend->setEnabled(true);
        }
        else
        {
            logError("Ошибка при открытии сервера");
        }
    }
    else
    {
        ui->pushButtonBind->setPalette(errPal);
    }
}

/// Слот установки соединения
void MainWindow::on_pushButtonConnect_clicked()
{
    // Проверка введённного с формы порта
    bool okPort1;
    quint16 port = quint16(ui->lineEditPort->text().toInt(&okPort1));
    if (okPort1)
        ui->lineEditPort->setPalette(normPal);
    else
        ui->lineEditPort->setPalette(errPal);

    if (okPort1)
    {
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
        sendSocket->connectToHost(ui->lineEditAddress->text(), port);

        // Добавляем новый сокет
        sockets.append(sendSocket);

        updateConnectionsIndicator();

        ui->checkBoxResend->setEnabled(false);
    }
    else
    {
        ui->pushButtonConnect->setPalette(errPal);
    }
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
                .arg(getSocket->localAddress().toString().split(":").last()));

        // Добавляем новый сокет
        sockets.append(getSocket);

        ui->lineEditMessage->setEnabled(true);
        ui->pushButtonSend->setEnabled(true);

        updateConnectionsIndicator();
    }
}

/// Слот, соединенённый с сигналом удачного соединения отправляющего сокета
void MainWindow::onTcpConnect()
{
    logInfo(QString("Соединение с <b>%1</b> установлено!")
        .arg(sockets[0]->localAddress().toString().split(":").last()));

    ui->lineEditMessage->setEnabled(true);
    ui->pushButtonSend->setEnabled(true);
}

/// Слот, соединённый с сигналом дисконнекта отправляющего сокета
void MainWindow::onTcpSendDisconnect()
{
    logWarn("Вы отключились от собеседника");

    clearSockets();

    updateConnectionsIndicator();
}

/// Слот, соединённый с сигналом дисконнекта принимающего сокета
void MainWindow::onTcpGetDisconnect()
{
    auto getSocket = static_cast<QTcpSocket *>(sender());
    if (getSocket != nullptr)
    {
        logWarn(QString("Собеседник <b>%1</b> отключился от вас")
                .arg(getSocket->localAddress().toString().split(":").last()));

        // Удаляем сокет из вектора сокетов
        sockets.removeOne(getSocket);
        getSocket->deleteLater();

        if (sockets.length() == 0)
        {
            // Если больше нет активных сокетов, отключаем возможность отправлять сообщения
            ui->lineEditMessage->setEnabled(false);
            ui->pushButtonSend->setEnabled(false);
        }
    }

    updateConnectionsIndicator();
}

/// Слот, соединённый с сигналом об ошибки отправляющего сокета
void MainWindow::onTcpSendError(QAbstractSocket::SocketError err)
{
    // Небольшая обработка "известных" ошибок сокета,
    // чтобы не вводить пользователя в заблуждение
    switch (err) {
    case QAbstractSocket::RemoteHostClosedError:
        logWarn("Вы отключились от сервера");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        logWarn("Не удаётся подключиться");
        break;
    default:
        logError("Ошибка сокета отправки");
    }

    auto socket = static_cast<QTcpSocket *>(sender());
    if (socket != nullptr)
    {
        // При ошибке закрываем отправляющий сокет
        socket->disconnect();
        socket->deleteLater();
        sockets.removeOne(socket);
    }

    ui->lineEditMessage->setEnabled(false);
    ui->pushButtonSend->setEnabled(false);

    updateConnectionsIndicator();
}

/// Слот, соединённый с сигналом приёма данных принимающего сокета
void MainWindow::onTcpReadyRead()
{
    auto socket = static_cast<QTcpSocket *>(sender());
    if (socket != nullptr)
    {
        // Считываем данные
        QString adr = socket->localAddress().toString().split(":").last();
        auto data = socket->readAll();
        QString str;
        if (ui->checkBoxUtf8->isChecked())
            str = QString::fromUtf8(data);
        else
            str = QString::fromLocal8Bit(data);

        // Выводим сообщение в "консоль"
        logMessage(str, adr, "C2145C");

        if (server != nullptr && ui->checkBoxResend->isChecked() && sockets.length() > 1)
        {
            // Если сейчас активен режим сервера, включена пересылка сообщений и к серверу подключено больше 1 пользователя,
            // то каждое приходящее сообщение будет автоматически переслано на все сокеты,
            // кроме того, с которого это сообщение пришло
            int socketCount = sockets.length();
            for (int i = 0; i < socketCount; i++)
            {
                if (sockets[i] != socket)
                    sockets[i]->write(data);
            }
        }
    }
}


/// Метод для очистки сокетов
void MainWindow::clearSockets()
{
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
    // Очищаем массив адресов на сокеты
    sockets.clear();

    updateConnectionsIndicator();
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
}


/// Метод для более удобной отправки сообщений
void MainWindow::sendMessage(QString message, QString author, QString clr)
{
    if (message.length() > 0)
    {
        // Отправка данных
        QByteArray data;
        if (ui->checkBoxUtf8->isChecked())
            data = message.toUtf8();
        else
            data = message.toLocal8Bit();

        int length = sockets.length();
        for (int i = 0; i < length; i++)
        {
            auto socket = sockets[i];
            socket->write(data);
        }

        ui->lineEditMessage->clear();
        ui->lineEditMessage->setFocus();
        // Вывод сообщения в "консоль"
        logMessage(message, author, clr);
    }
}


/// Обновление кнопки с количеством активных подключений
void MainWindow::updateConnectionsIndicator()
{
    int socketsCount = sockets.length();
    ui->pushButtonConnectionList->setText(QString("Подключено: %1").arg(socketsCount));
    if (socketsCount > 0)
        ui->pushButtonConnectionList->setEnabled(true);
    else
        ui->pushButtonConnectionList->setEnabled(false);
}


/// Слот отображения списка подключений
void MainWindow::on_pushButtonConnectionList_clicked()
{
    int socketCount = sockets.length();
    logInfo(QString("Подключено к <b>%1</b> %2:").arg(socketCount)
            .arg(socketCount % 10 == 1 ? "сокету" : "сокетам"));
    // Вывод списка всех подключений
    for (int i = 0; i < socketCount; i++)
    {
        logInfo(QString::number(i + 1) + ") <b>" + sockets[i]->localAddress().toString().split(":").last() + "</b>");
    }
}


/// Вывод "страшного" красного сообщения в "консоль"
void MainWindow::logError(QString text)
{
    log("<span style=\"color: #F8173E\">" + text + "</span>");
}

/// Вывод предупрежения в "консоль"
void MainWindow::logWarn(QString text)
{
    log("<span style=\"color: #FFC618\">" + text + "</span>");
}

/// Вывод "дополнительной" информации в "консоль"
void MainWindow::logInfo(QString text)
{
    log("<span style=\"color: #4682B4\">" + text + "</span>");
}

/// Вывод сообщения в "консоль"
void MainWindow::logMessage(QString content, QString author, QString clr)
{
    QString ctStr = QTime::currentTime().toString("hh:mm:ss");
    log(QString("<b>[<span style=\"color: #%1\">%2</span>]</b> >> %3  "
                "<i style=\"color: #707070; font-size: 8pt;\">(%4)</i>")
        .arg(clr, author, content, ctStr));
}

/// Вывод текста в "консоль", сокращение для ui->chatTextEdit->append
void MainWindow::log(QString text)
{
    if (ui != nullptr)
        ui->chatTextEdit->append(text);
}


/// Слот автоматической отправки сообщения
void MainWindow::onSpam()
{
    if (ui->checkBoxSpam->isChecked() &&
            ui->lineEditMessage->isEnabled())
    {
        QString str = "Hello World! Привет, мир!";

        // Вывод сообщение о спаме
        sendMessage(str, "СПАМ", "F8173E");
    }
}

/// Слот очистки чата
void MainWindow::on_pushButtonClear_clicked()
{
    ui->chatTextEdit->clear();
}

