#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpSocket>

namespace Ui {
class ClientWindow;
}

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void on_pushButtonConnect_clicked();

    void onSocketConnected();
    void onSocketError(QAbstractSocket::SocketError err);
    void onSocketReadyRead();

    void on_pushButtonSend_clicked();
    void on_pushButtonClear_clicked();

    void on_pushButtonLeave_clicked();

private:
    Ui::ClientWindow *ui;

    QTcpSocket *socket{nullptr};

    void formActionsConnect();
    void formActionsDisconnect();

    void sendMessage(QString text);

    QString addressToString(QHostAddress addr);
    QHostAddress byteArrayToAddress(QByteArray ba);

    void logError(QString text);
    void logWarn(QString text);
    void logInfo(QString text);
    void log(QString text);
};

#endif // CLIENTWINDOW_H
