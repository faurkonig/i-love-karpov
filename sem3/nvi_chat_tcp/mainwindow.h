#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QVector>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonSend_clicked();
    void on_pushButtonBind_clicked();
    void on_pushButtonConnect_clicked();
    void on_pushButtonConnectionList_clicked();
    void on_pushButtonClear_clicked();
    
    void on_radioButtonServer_clicked();
    void on_radioButtonClient_clicked();

    void onTcpBinded();
    void onTcpConnect();
    void onTcpSendDisconnect();
    void onTcpGetDisconnect();
    void onTcpSendError(QAbstractSocket::SocketError err);
    void onTcpReadyRead();

    void onSpam();

    void on_pushButtonAddresses_clicked();

private:
    Ui::MainWindow *ui;

    QPalette normPal;
    QPalette errPal;

    QTimer spamTimer;

    QTcpServer *server = nullptr;
//    QTcpSocket *sendSocket = nullptr;
//    QTcpSocket *getSocket = nullptr;
    QVector<QTcpSocket *> sockets;

    bool isError;

    void clearSockets();
    void clearServer();

    void sendMessage(QString message, QString author, QString clr = "FFFFFF");

    void updateConnectionsIndicator();

    void logError(QString text);
    void logWarn(QString text);
    void logInfo(QString text);
    void logMessage(QString content, QString author, QString clr = "FFFFFF");
    void log(QString text);
};
#endif // MAINWINDOW_H
