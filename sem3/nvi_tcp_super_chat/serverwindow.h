#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QVector>

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void on_pushButtonBind_clicked();

    void onServerNewConnection();
    void onSocketError(QAbstractSocket::SocketError err);
    void onSocketReadyRead();

    void on_pushButtonSend_clicked();
    void on_pushButtonAddresses_clicked();
    void on_pushButtonClear_clicked();

    void on_pushButtonLeave_clicked();

private:
    Ui::ServerWindow *ui;

    QTcpServer *server{nullptr};

    QVector<QTcpSocket *> sockets;

    void sendMessage(QString text);

    QString addressToString(QHostAddress addr);
    QByteArray addressToByteArray(QHostAddress addr);

    void logError(QString text);
    void logWarn(QString text);
    void logInfo(QString text);
    void log(QString text);
};

#endif // SERVERWINDOW_H
