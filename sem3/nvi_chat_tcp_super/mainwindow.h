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
    void scrollAreaRangeChanged();

    void on_pushButtonSend_clicked();
    void on_pushButtonBind_clicked();
    void on_pushButtonConnect_clicked();
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
    QVector<QTcpSocket *> sockets;
    QVector<QString> userNicknames;

    bool isError{false};
    bool isClearing{false};

    QString userConnectionNick;

    QVector<QWidget *> chatWidgets;

    void processServerData(QByteArray data, QTcpSocket *socket);
    void processClientData(QByteArray data, QTcpSocket *socket);

    void clearSockets();
    void clearServer();

    void sendMessage(QString message, QString author, QString clr = "000000");

    QString addressToString(QHostAddress address);
    QByteArray addressToBytes(QHostAddress address);
    QHostAddress bytesToAddress(QByteArray ba);
    QString bytesToNick(QByteArray ba, int &byteLength);

    QString getEnteredNickname();

    void logMessage(QString content, QString author, QString color = "000000");
    void logError(QString content);
    void logWarn(QString content);
    void logInfo(QString content);
    QWidget *genLabelWidget(QString content, int r, int g, int b, int a);
    void addWidgetToChat(QWidget *widget);
};
#endif // MAINWINDOW_H
