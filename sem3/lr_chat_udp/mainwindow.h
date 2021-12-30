#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QUdpSocket> //библиотека работы с UDP
#include <QMessageBox>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_Create_clicked();
    void on_pushButton_SendMes_clicked();

    void read_data(); //слот для обработки поступивших данных
    void spam_timeout(); // слот для отправки спама

    void on_pushButton_Address_clicked();

private:
    Ui::MainWindow *ui;

    QUdpSocket *udpsocket = nullptr; //сам UDP сокет (указатель на него)
    QHostAddress *address = nullptr; //для адреса
    int port, our_port; //порты

    QTimer spamTimer;
};

#endif // MAINWINDOW_H
