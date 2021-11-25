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
    void on_pushButtonSendImage_clicked();
    void on_pushButtonSendFile_clicked();
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

    void on_timerIntervalSpinBox_valueChanged(double arg1);
    void onSpam();

    void on_pushButtonAddresses_clicked();

    void openFileButtonClicked();
    void showFileButtonClicked();


private:
    Ui::MainWindow *ui;

    QPalette normPal;
    QPalette errPal;

    QTimer spamTimer;

    QTcpServer *server{nullptr};
    QVector<QTcpSocket *> sockets;
    QVector<QString> userNicknames;

    bool isError{false};
    bool isClearing{false};

    QString userConnectionNick;
    int currentBlockSize{-1};

    QVector<QWidget *> chatWidgets;

    void startServer();
    void stopServer();
    void startClient();
    void stopClient();

    void processPendingData(QTcpSocket *senderSocket);
    void processServerData(QByteArray data, QTcpSocket *socket);
    void processClientData(QByteArray data, QTcpSocket *socket);

    void clearSockets();
    void clearServer();

    void sendData(QByteArray data, QTcpSocket *exception = nullptr);
    void sendMessage(QString message);
    void sendImage(QString filePath);
    void sendFile(QString filePath);

    QString addressToString(QHostAddress address);
    QByteArray addressToBytes(QHostAddress address);
    QHostAddress bytesToAddress(QByteArray ba);
    QByteArray intToBytes(quint32 value);
    quint32 bytesToInt(QByteArray ba);

    QString getEnteredNickname();
    bool isServerMode();

    QWidget *logTextMessage(QString content, QString author, QString color = "000000");
    QWidget *logImageMessage(QPixmap pixmap, QString filePath, QString author, QString color = "000000");
    QWidget *logFileMessage(QString filePath, QString author, QString color = "000000");
    QWidget *logError(QString content);
    QWidget *logWarn(QString content);
    QWidget *logInfo(QString content);
    QWidget *genLabelWidget(QString content, int r, int g, int b, int a);
    void addWidgetToChat(QWidget *widget);

    QString getFilenameForDownload(QString name);
    void showInFolder(QString filePath);
    void openFile(QString filePath);
};
#endif // MAINWINDOW_H
