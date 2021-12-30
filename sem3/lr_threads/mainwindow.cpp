#include "mainwindow.h"
#include "ui_mainwindow.h"

//Конструктор
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Простые потоки
    thrd1 = new my_thread();
    thrd2 = new my_thread();
    thrd3 = new my_thread();
    srand(200);

    llt1= new ll_thread();
    llt1->start();

    connect(llt1,SIGNAL(status_upd()),this,SLOT(potok_info()));

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        ll_thread *ll = new ll_thread(i + 1);
        connect(ll, &ll_thread::status_upd, this, &MainWindow::multi_potok_info);
        ll->start();
        mllts[i] = ll;
    }
}

//Деструктор
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Run_clicked() //запуск простых потоков
{
    thrd1->stch=rand()%10000; //записываем в счетчик потока случайное число в диапазоне от 0 до 9999
    thrd1->start();           //запускаем поток на выполнение (будет работать переопределенный метод run)
    thrd2->stch=rand()%10000;
    thrd2->start();
    thrd3->stch=rand()%10000;
    thrd3->start();
}

void MainWindow::on_pushButton_Show_Stat_clicked() //поток начинается выполнять действия
{
    ui->textEdit->append("1: "+((thrd1->stch>0)?QString::number(thrd1->stch)+" msec":"worked")+
                         "\n2: "+((thrd2->stch>0)?QString::number(thrd2->stch)+" msec":"worked")+ \
                         "\n3: "+((thrd3->stch>0)?QString::number(thrd3->stch)+" msec":"worked"));
}

void MainWindow::on_pushButton_ll_work_clicked()// запуск на работу
{
    llt1->com=1;
    llt1->mtx->unlock();
    ui->pushButton_ll_kill->setEnabled(false);
    ui->pushButton_ll_work->setEnabled(false);
}

void MainWindow::on_pushButton_ll_kill_clicked()//запуск на самоуничтожение
{
    llt1->com=2;
    llt1->mtx->unlock();
    ui->pushButton_ll_kill->setEnabled(false);
    ui->pushButton_ll_work->setEnabled(false);
}

void MainWindow::potok_info()//инфо о потоке
{
    if (llt1->com == 3)
    {
        ui->textEdit->append("Potok Stopped: " + QString::number(llt1->stch)+ " msec");// выполнение

        ui->pushButton_ll_kill->setEnabled(true); //кнопка
        ui->pushButton_ll_work->setEnabled(true);
    }
    else if (llt1->com == 4)
    {
        ui->textEdit->append("Potok Killed: " + QString::number(llt1->stch)+ " msec");//завершение
    }
}

void MainWindow::multi_potok_info()//инфо о всех 3 потоках
{
    ll_thread *thr = static_cast<ll_thread *>(sender());
    int num = thr->getNum();
    if (thr->com == 3)
    {
        ui->textEdit->append("Potok "+QString::number(num)+" Stopped: "+QString::number(thr->stch)+" msec"); //выполнение потока
    }
    else if (thr->com == 4)
    {
        ui->textEdit->append("Potok "+QString::number(num)+" Killed: "+QString::number(thr->stch)+" msec");// выполняется если поток больше 3и завершение потока
    }

    n++;

    if (n == THREAD_COUNT)
    {
        if (thr->com == 3)
        {
            ui->textEdit->append("Все потоки завершились"); //вывод

            ui->pushButton_multi_ll_work->setEnabled(true);
            ui->pushButton_multi_ll_kill->setEnabled(true);
        }
        else if (thr->com == 4)
        {
            ui->textEdit->append("Все потоки умерли"); //вывод
        }
    }
}

void MainWindow::on_pushButton_multi_ll_work_clicked()//запуск нескольких потоков одновременно на работу
{
    n = 0;

    for (int i = 0; i < THREAD_COUNT; i++) //посылается команда работать и разблокируется поток
    {
        mllts[i]->com = 1;
        mllts[i]->mtx->unlock();
    }

    ui->pushButton_multi_ll_work->setEnabled(false); //кнопка становится неактивной
    ui->pushButton_multi_ll_kill->setEnabled(false); //кнопка становится неактивной
}

void MainWindow::on_pushButton_multi_ll_kill_clicked()//запуск нескольких потоков одновременно на самоуничтожение
{
    n = 0;

    for (int i = 0; i < THREAD_COUNT; i++) //посылается программа работать
    {
        mllts[i]->com = 2;
        mllts[i]->mtx->unlock();
    }

    ui->pushButton_multi_ll_work->setEnabled(false); //кнопка становится неактивной
    ui->pushButton_multi_ll_kill->setEnabled(false); //кнопка становится неактивной
}
