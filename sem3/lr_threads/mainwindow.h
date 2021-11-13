#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "my_thread.h"
#include "ll_thread.h"

#define THREAD_COUNT 3

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void potok_info();
    void multi_potok_info();

private slots:
    void on_pushButton_Run_clicked();

    void on_pushButton_Show_Stat_clicked();

    void on_pushButton_ll_work_clicked();

    void on_pushButton_ll_kill_clicked();

    void on_pushButton_multi_ll_work_clicked();

    void on_pushButton_multi_ll_kill_clicked();

private:
    Ui::MainWindow *ui;

    int n;

    my_thread *thrd1;
    my_thread *thrd2;
    my_thread *thrd3;

    ll_thread *llt1;

//    ll_thread *mllt1;
//    ll_thread *mllt2;
//    ll_thread *mllt3;

    ll_thread *mllts[THREAD_COUNT];
};

#endif // MAINWINDOW_H
