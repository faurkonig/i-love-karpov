#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MULTITHREAD_COUNT 3

#include <QMainWindow>
#include "oneshotthread.h"
#include "loopthread.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    OneshotThread *oneshotThread1;
    OneshotThread *oneshotThread2;

    LoopThread *lThread;

    LoopThread *multiThreads[MULTITHREAD_COUNT];

private slots:
    void on_pushButtonRunSimple_clicked();
    void on_pushButtonShowStatsSimple_clicked();

    void on_pushButtonLoopWork_clicked();
    void on_pushButtonLoopKill_clicked();
    void on_pushButtonLoopRespawn_clicked();

    void on_pushButtonMultiThreadWork_clicked();
    void on_pushButtonMultiThreadKill_clicked();
    void on_pushButtonMultiThreadRespawn_clicked();

    void threadInfo();
    void multiThreadInfo();

    void on_pushButtonClear_clicked();

    void on_pushButtonMultiThreadStats_clicked();

private:
    Ui::MainWindow *ui;

    void initLoopThread();
    void initMultiThread();

    void setMultiThreadCommand(int command);
    void unlockMultiThreadMutex();
    bool isMultiThreadStopped();
    bool isMultiThreadKilled();

    void log(QString text);
    void logOneshotState(int number, OneshotThread *thread);
    void logLoopState(LoopThread *thread);

    void disableLLButtons();
    void enableLLButtons();
    void hideLLButtons();
    void showLLButtons();

    void disableMTButtons();
    void enableMTButtons();
    void hideMTButtons();
    void showMTButtons();
};
#endif // MAINWINDOW_H
