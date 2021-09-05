#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
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
    void onMainTimerTimeout();
    void onDownTimerTimeout();

    void on_startButton_clicked();
    void on_pauseButton_clicked();
    void on_stopButton_clicked();
    void on_armagedonButton_clicked();

private:
    Ui::MainWindow *ui;

    QTimer *mainTimer;
    int mainCounter{0};
    bool isMainTimerStoped{true};

    QTimer *downTimer;
    int downCounter{0};

    QString formatNumberAsTime(int number);
    QString formatDigit(int digit);
};
#endif // MAINWINDOW_H
