#include <QtNetwork/QHostAddress>
#include <QTime>
#include "clientwindow.h"
#include "ui_clientwindow.h"
#include "mainwindow.h"

ClientWindow::ClientWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ClientWindow)
{
    ui->setupUi(this);

    connect(ui->lineEditMessage, &QLineEdit::returnPressed, this, &ClientWindow::on_pushButtonSend_clicked);

    formActionsDisconnect();
}

ClientWindow::~ClientWindow()
{
    if (socket != nullptr)
    {
        if (socket->isOpen())
        {
            socket->close();
            socket->disconnect();
        }
        socket->waitForDisconnected();
        delete socket;
    }

    delete ui;
}

void ClientWindow::on_pushButtonConnect_clicked()
{
    if (socket == nullptr)
    {
        int port = ui->lineEditPort->text().toInt();

        if (port > 0 && port < 0x10000)
        {
            // Если подключение ещё не установлено
            socket = new QTcpSocket(this);

            connect(socket, &QTcpSocket::connected, this, &ClientWindow::onSocketConnected);
#if QT_VERSION_MAJOR >= 5 && QT_VERSION_MINOR >= 15
            connect(socket, &QTcpSocket::errorOccurred, this, &ClientWindow::onSocketError);
#else
            connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &ClientWindow::onSocketError);
#endif
            connect(socket, &QTcpSocket::readyRead, this, &ClientWindow::onSocketReadyRead);

            ui->pushButtonConnect->setEnabled(false);

            socket->connectToHost(ui->lineEditAddress->text(), quint16(port));
        }
        else
        {
            logError("Введён неправильный порт!");
        }
    }
    else
    {
        logInfo("Вы отключились от сервера");

        // Если подключение уже активно
        if (socket->isOpen())
        {
            socket->close();
            socket->disconnect();
        }
        socket->deleteLater();
        socket = nullptr;

        formActionsDisconnect();
    }
}

void ClientWindow::onSocketConnected()
{
    logInfo(QString("Подключение с сервером <b>%1</b> установлено")
            .arg(addressToString(socket->localAddress())));

    ui->pushButtonConnect->setEnabled(true);
    formActionsConnect();
}

void ClientWindow::onSocketError(QAbstractSocket::SocketError err)
{

    if (socket != nullptr)
    {
        switch (err) {
        case QAbstractSocket::RemoteHostClosedError:
            logWarn("Соединение с сервером разорвано");
            break;
        case QAbstractSocket::ConnectionRefusedError:
            logError("Подключение отклонено");
            break;
        case QAbstractSocket::HostNotFoundError:
            logError("Хост не найден");
            break;
        case QAbstractSocket::NetworkError:
            logError("Ошибка сети");
            break;
        default:
            logError("Ошибка сокета отправки");
        }

        socket->deleteLater();
        socket = nullptr;

        formActionsDisconnect();
    }
}

void ClientWindow::onSocketReadyRead()
{
    auto data = socket->readAll();
    if (data.size() > 0)
    {
        switch (data[0]) {
        case char(0): { // Если сообщение было отправлено именно сервером
            // Считывание текста
            QString text = QString::fromUtf8(data.mid(1));

            QString ctStr = QTime::currentTime().toString("hh:mm:ss");
            log(QString("<b>[<span style=\"color: #C2145C\">Сервер</span>]</b> >> %1  "
                        "<i style=\"color: #707070; font-size: 8pt;\">(%2)</i>")
                .arg(text, ctStr));
            break;
        }
        case char(1): { // Если сообщение было отправлено кем-то из клиентов
            // Считывание IP-адреса
            auto addr = byteArrayToAddress(data.mid(1, 4));
            // Считывание текста
            QString text = QString::fromUtf8(data.mid(5));

            QString ctStr = QTime::currentTime().toString("hh:mm:ss");
            log(QString("<b>[<span style=\"color: #C2145C\">%1</span>]</b> >> %2  "
                        "<i style=\"color: #707070; font-size: 8pt;\">(%3)</i>")
                .arg(addressToString(addr), text, ctStr));
            break;
        }
        case char(2): { // Кто-то из клиентов подключился
            // Считывание IP-адреса
            auto addr = byteArrayToAddress(data.mid(1, 4));

            logInfo(QString("<b>%1</b> подключился к беседе")
                    .arg(addressToString(addr)));
            break;
        }
        case char(3): { // Кто-то из клиентов отключился
            // Считывание IP-адреса
            auto addr = byteArrayToAddress(data.mid(1, 4));

            logInfo(QString("<b>%1</b> отключился от беседы")
                    .arg(addressToString(addr)));
            break;
        }
        }
    }
}


void ClientWindow::on_pushButtonSend_clicked()
{
    auto str = ui->lineEditMessage->text();
    if (str.length() > 0)
    {
        sendMessage(str);
        ui->lineEditMessage->clear();
        ui->lineEditMessage->setFocus();
    }
}

void ClientWindow::on_pushButtonClear_clicked()
{
    ui->textEditChat->clear();
}


void ClientWindow::formActionsConnect()
{
    ui->lineEditMessage->setEnabled(true);
    ui->pushButtonSend->setEnabled(true);

    ui->pushButtonConnect->setText("Отключиться");
}

void ClientWindow::formActionsDisconnect()
{
    ui->lineEditMessage->clear();
    ui->lineEditMessage->setEnabled(false);
    ui->pushButtonSend->setEnabled(false);

    ui->pushButtonConnect->setText("Подключиться");
    ui->pushButtonConnect->setEnabled(true);
}


void ClientWindow::sendMessage(QString text)
{
    socket->write(text.toUtf8());

    QString ctStr = QTime::currentTime().toString("hh:mm:ss");
    log(QString("<b>[<span style=\"color: #8314C2\">Вы</span>]</b> >> %1  "
                "<i style=\"color: #707070; font-size: 8pt;\">(%2)</i>")
        .arg(text, ctStr));
}


QString ClientWindow::addressToString(QHostAddress addr)
{
    return addr.toString().split(':').last();
}

QHostAddress ClientWindow::byteArrayToAddress(QByteArray ba)
{
    quint32 addrInt;
    memcpy(&addrInt, ba.constData(), 4);
    return QHostAddress(addrInt);
}


void ClientWindow::logError(QString text)
{
    log("<span style=\"color: #F8173E\">" + text + "</span>");
}

void ClientWindow::logWarn(QString text)
{
    log("<span style=\"color: #FFC618\">" + text + "</span>");
}

void ClientWindow::logInfo(QString text)
{
    log("<span style=\"color: #4682B4\">" + text + "</span>");
}

void ClientWindow::log(QString text)
{
    ui->textEditChat->append(text);
}


void ClientWindow::on_pushButtonLeave_clicked()
{
    (new MainWindow)->show();
    close();
}
