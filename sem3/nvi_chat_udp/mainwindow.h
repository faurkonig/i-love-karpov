#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket>

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
    void on_pushButtonConnect_clicked();

    void onUdpConnect();
    void onUdpError(QAbstractSocket::SocketError err);
    void onUdpReadyRead();

private:
    Ui::MainWindow *ui;

    QPalette normPal;
    QPalette errPal;

    QUdpSocket *udpSocket = nullptr;

    void logError(QString text);
    void log(QString text);
};
#endif // MAINWINDOW_H
