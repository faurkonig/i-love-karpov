#include <QTime>
#include <QtNetwork/QNetworkInterface>
#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "mainwindow.h"

ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

    connect(ui->lineEditMessage, &QLineEdit::returnPressed, this, &ServerWindow::on_pushButtonSend_clicked);
}

ServerWindow::~ServerWindow()
{
    // Закрытие и удаление сервера
    if (server != nullptr)
    {
        if (server->isListening())
            server->close();
        delete server;
    }
    // Закрытие и удаление сокетов
#if QT_VERSION_MAJOR >= 5 && QT_VERSION_MINOR >= 7
    for (const auto socket : qAsConst(sockets))
#else
    for (const auto socket : sockets)
#endif
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


void ServerWindow::on_pushButtonBind_clicked()
{
    if (server == nullptr)
    {
        // Если сервер ещё не открыт
        int port = ui->lineEditPort->text().toInt();
        if (port > 0 && port < 0x10000)
        {
            server = new QTcpServer(this);
            connect(server, &QTcpServer::newConnection, this, &ServerWindow::onServerNewConnection);
            if (server->listen(QHostAddress::Any, quint16(port)))
            {
                ui->pushButtonBind->setText("Выключить сервер");

                logInfo(QString("Сервер прослушивает порт <b>%1</b>")
                        .arg(port));
            }
            else
            {
                server->deleteLater();
                server = nullptr;

                logError("Ошибка при открытии сервера");
            }
        }
        else
        {
            logError("Введён неправильный порт!");
        }
    }
    else
    {
        // Если сервер уже открыт
        // Очистка сокетов
        // Здесь это сделано по этой причине: https://github.com/KDE/clazy/blob/1.10/docs/checks/README-range-loop-detach.md
#if QT_VERSION_MAJOR >= 5 && QT_VERSION_MINOR >= 7
        for (const auto socket : qAsConst(sockets))
#else
        for (const auto socket : sockets)
#endif
        {
            if (socket->isOpen())
            {
                socket->close();
                socket->disconnect();
            }
            socket->deleteLater();
        }

        // Очистка сервера
        if (server->isListening())
            server->close();
        server->deleteLater();
        server = nullptr;

        ui->pushButtonBind->setText("Включить сервер");

        logInfo("Сервер выключен");
    }
}


void ServerWindow::onServerNewConnection()
{
    auto newSocket = server->nextPendingConnection();
    if (newSocket != nullptr)
    {
#if QT_VERSION_MAJOR >= 5 && QT_VERSION_MINOR >= 15
        connect(newSocket, &QTcpSocket::errorOccurred, this, &ServerWindow::onSocketError);
#else
        connect(newSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &ServerWindow::onSocketError);
#endif
        connect(newSocket, &QTcpSocket::readyRead, this, &ServerWindow::onSocketReadyRead);

        sockets.append(newSocket);

        logInfo(QString("К серверу подключился <b>%1</b>")
                .arg(addressToString(newSocket->localAddress())));

        auto disconnectData = addressToByteArray(newSocket->localAddress());
        disconnectData.prepend(char(2));
#if QT_VERSION_MAJOR >= 5 && QT_VERSION_MINOR >= 7
        for (const auto socket : qAsConst(sockets))
#else
        for (const auto socket : sockets)
#endif
        {
            if (socket != newSocket)
            {
                socket->write(disconnectData);
            }
        }
    }
}

void ServerWindow::onSocketError(QAbstractSocket::SocketError err)
{
    auto senderSocket = static_cast<QTcpSocket *>(sender());
    if (senderSocket != nullptr)
    {

        switch (err) {
        case QAbstractSocket::RemoteHostClosedError:
            logWarn(QString("Собеседник <b>%1</b> отключился от вас")
                    .arg(addressToString(senderSocket->localAddress())));
            break;
        default:
            logError(QString("Ошибка на сокете <b>%1</b>")
                     .arg(addressToString(senderSocket->localAddress())));
        }

        auto disconnectData = addressToByteArray(senderSocket->localAddress());
        disconnectData.prepend(char(3));
#if QT_VERSION_MAJOR >= 5 && QT_VERSION_MINOR >= 7
        for (const auto socket : qAsConst(sockets))
#else
        for (const auto socket : sockets)
#endif
        {
            if (socket != senderSocket)
            {
                socket->write(disconnectData);
            }
        }

        senderSocket->disconnect();
        senderSocket->deleteLater();
        sockets.removeOne(senderSocket);
    }
}

void ServerWindow::onSocketReadyRead()
{
    auto senderSocket = static_cast<QTcpSocket *>(sender());
    if (senderSocket != nullptr)
    {
        auto data = senderSocket->readAll();
        if (data.size() > 0)
        {
            auto text = QString::fromUtf8(data);
            QString ctStr = QTime::currentTime().toString("hh:mm:ss");

            log(QString("<b>[<span style=\"color: #C2145C\">%1</span>]</b> >> %2  "
                        "<i style=\"color: #707070; font-size: 8pt;\">(%3)</i>")
                .arg(addressToString(senderSocket->localAddress()), text, ctStr));

            auto addrAsBytes = addressToByteArray(senderSocket->localAddress());
#if QT_VERSION_MAJOR >= 5 && QT_VERSION_MINOR >= 7
            for (const auto socket : qAsConst(sockets))
#else
            for (const auto socket : sockets)
#endif
            {
                if (socket != senderSocket)
                {
                    data.prepend(addrAsBytes);
                    data.prepend(char(1));
                    socket->write(data);
                }
            }
        }
    }
}


void ServerWindow::on_pushButtonAddresses_clicked()
{
    // Получаем все адреса
    auto adrs = QNetworkInterface::allAddresses();
    logInfo("IP адреса компьютера:");
    for (int i = 0; i < adrs.count(); i++)
    {
        // Отсеиваем только адреса типа IPv4
        if (adrs[i].protocol() == QAbstractSocket::IPv4Protocol)
            logInfo(QString("%1) <b>%2</b>").arg(i + 1)
                    .arg(addressToString(adrs[i])));
    }
}

void ServerWindow::on_pushButtonSend_clicked()
{
    auto str = ui->lineEditMessage->text();
    if (str.length() > 0)
    {
        sendMessage(str);
        ui->lineEditMessage->clear();
        ui->lineEditMessage->setFocus();
    }
}

void ServerWindow::on_pushButtonClear_clicked()
{
    ui->textEditChat->clear();
}


void ServerWindow::sendMessage(QString text)
{
#if QT_VERSION_MAJOR >= 5 && QT_VERSION_MINOR >= 7
    for (const auto socket : qAsConst(sockets))
#else
    for (const auto socket : sockets)
#endif
    {
        auto data = text.toUtf8();
        data.prepend(char(0));
        socket->write(data);
    }

    QString ctStr = QTime::currentTime().toString("hh:mm:ss");
    log(QString("<b>[<span style=\"color: #8314C2\">Вы</span>]</b> >> %1  "
                "<i style=\"color: #707070; font-size: 8pt;\">(%2)</i>")
        .arg(text, ctStr));
}


QString ServerWindow::addressToString(QHostAddress addr)
{
    return addr.toString().split(':').last();
}

QByteArray ServerWindow::addressToByteArray(QHostAddress addr)
{
    char *addrAsBytes = new char[4];
    quint32 addrIPv4 = addr.toIPv4Address();
    memcpy(addrAsBytes, &addrIPv4, 4);
    QByteArray ba(addrAsBytes, 4);
    delete [] addrAsBytes;
    return ba;
}


void ServerWindow::logError(QString text)
{
    log("<span style=\"color: #F8173E\">" + text + "</span>");
}

void ServerWindow::logWarn(QString text)
{
    log("<span style=\"color: #FFC618\">" + text + "</span>");
}

void ServerWindow::logInfo(QString text)
{
    log("<span style=\"color: #4682B4\">" + text + "</span>");
}

void ServerWindow::log(QString text)
{
    ui->textEditChat->append(text);
}


void ServerWindow::on_pushButtonLeave_clicked()
{
    (new MainWindow)->show();
    this->~ServerWindow();
}
