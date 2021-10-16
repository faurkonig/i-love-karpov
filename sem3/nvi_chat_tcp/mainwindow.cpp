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
}

/// Деструктор
MainWindow::~MainWindow()
{
    delete ui;
}


/// Отправка введённого сообщения
void MainWindow::on_pushButtonSend_clicked()
{
    auto str = ui->lineEditMessage->text();
    if (str.length() > 0)
    {
        // Отправка данных
        QByteArray data;
        if (ui->checkBoxUtf8->isChecked())
            data = str.toUtf8();
        else
            data = str.toLocal8Bit();
        sendSocket->write(data);

        if (sendSocket->waitForBytesWritten(data.length()))
        {
            // Вывод сообщения в "консоль"
            logMessage(str, "Вы", "8314C2");

            ui->lineEditMessage->clear();
            ui->lineEditMessage->setFocus();
        }
    }
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

        // Если сервер уже был создан, закрываем его, отправляем на удаление и создаём новый
        if (server != nullptr)
        {
            if (server->isListening())
                server->close();
            server->deleteLater();
        }
        server = new QTcpServer(this);
        connect(server, &QTcpServer::newConnection, this, &MainWindow::onTcpBinded);
        if (server->listen(QHostAddress::Any, localPort))
        {
            logInfo("Прослушиваем порт <b>" + QString::number(localPort) + "</b>");
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
        // Если сокет для отправки уже был создан, то отключаемся, отправляем на удаление и создаём новый
        if (sendSocket != nullptr)
        {
            if (sendSocket->isOpen())
                sendSocket->disconnectFromHost();
            sendSocket->deleteLater();
        }
        sendSocket = new QTcpSocket(this);
        connect(sendSocket, &QTcpSocket::connected, this, &MainWindow::onTcpConnect);
        connect(sendSocket, &QTcpSocket::disconnected, this, &MainWindow::onTcpSendDisconnect);
        // В версии Qt 5.15 добавили сигнал errorOccured, который является заменой ныне устаревшего сигнала error
#if QT_VERSION_MAJOR >= 5 && QT_VERSION_MINOR >= 15
        connect(sendSocket, &QTcpSocket::errorOccurred, this, &MainWindow::onTcpSendError);
#else
        connect(udpSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &MainWindow::onUdpError);
#endif
        sendSocket->connectToHost(ui->lineEditAddress->text(), port);
    }
    else
    {
        ui->pushButtonConnect->setPalette(errPal);
    }
}


/// Слот, соединённый с сигналом сервера, создаёт новое принимающее соединение
void MainWindow::onTcpBinded()
{
    // Если сокет для приёма был уже создан, то отключаемся, отправляем на удаление и создаём новый
    if (getSocket != nullptr)
    {
        if (getSocket->isOpen())
            getSocket->disconnect();
        getSocket->deleteLater();
    }
    getSocket = server->nextPendingConnection();
    if (getSocket != nullptr)
    {
        connect(getSocket, &QTcpSocket::disconnected, this, &MainWindow::onTcpGetDisconnect);
        connect(getSocket, &QTcpSocket::readyRead, this, &MainWindow::onTcpReadyRead);

        logInfo(QString("К серверу подключился <b>%1</b>!")
            .arg(getSocket->localAddress().toString().split(":").last()));
    }
}

/// Слот, соединенённый с сигналом удачного соединения отправляющего сокета
void MainWindow::onTcpConnect()
{
    logInfo(QString("Соединение с <b>%1</b> установлено!")
        .arg(sendSocket->localAddress().toString().split(":").last()));

    ui->lineEditMessage->setEnabled(true);
    ui->pushButtonSend->setEnabled(true);
}

/// Слот, соединённый с сигналом дисконнекта отправляющего сокета
void MainWindow::onTcpSendDisconnect()
{
    logWarn("Вы отключились от собеседника");

    sendSocket->deleteLater();
    sendSocket = nullptr;
}

/// Слот, соединённый с сигналом дисконнекта принимающего сокета
void MainWindow::onTcpGetDisconnect()
{
    logWarn("Собеседник отключился от вас");

    getSocket->deleteLater();
    getSocket = nullptr;
}

/// Слот, соединённый с сигналом об ошибки отправляющего сокета
void MainWindow::onTcpSendError(QAbstractSocket::SocketError err)
{
    // Небольшая обработка "известных" ошибок сокета,
    // чтобы не вводить пользователя в заблуждение
    switch (err) {
    case QAbstractSocket::RemoteHostClosedError:
        logWarn("Вы отключились от собеседника");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        logWarn("Не удаётся подключиться");
        break;
    default:
        logError("Ошибка сокета отправки");
    }

    // При ошибке закрываем отправляющий сокет
    sendSocket->disconnect();
    sendSocket->deleteLater();
    sendSocket = nullptr;

    ui->lineEditMessage->setEnabled(false);
    ui->pushButtonSend->setEnabled(false);
}

/// Слот, соединённый с сигналом приёма данных принимающего сокета
void MainWindow::onTcpReadyRead()
{
    // Считываем данные
    QString adr = getSocket->localAddress().toString().split(":")[3];
    auto data = getSocket->readAll();
    QString str;
    if (ui->checkBoxUtf8->isChecked())
        str = QString::fromUtf8(data);
    else
        str = QString::fromLocal8Bit(data);

    // Выводим сообщение в "консоль"
    logMessage(str, adr, "C2145C");
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
    ui->chatTextEdit->append(text);
}

/// Слот автоматической отправки сообщения по истечению времени таймера
void MainWindow::onSpam()
{
    if (ui->checkBoxSpam->isChecked() &&
            ui->lineEditMessage->isEnabled() &&
            sendSocket->isOpen())
    {
        QString str = "Hello World! Привет, мир!";
        // Отправка данных
        QByteArray data;
        if (ui->checkBoxUtf8->isChecked())
            data = str.toUtf8();
        else
            data = str.toLocal8Bit();
        sendSocket->write(data);

        // Вывод сообщение о спаме
        logMessage(str, "СПАМ", "F8173E");
    }
}
