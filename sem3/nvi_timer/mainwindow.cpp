#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mainTimer = new QTimer(this);
    connect(mainTimer, &QTimer::timeout, this, &MainWindow::onMainTimerTimeout);
    mainTimer->setInterval(10);
    ui->mainTimerLabel->display("00:00:00:00");

    downTimer = new QTimer(this);
    connect(downTimer, &QTimer::timeout, this, &MainWindow::onDownTimerTimeout);
    downTimer->setInterval(10);
    ui->downTimerLabel->display("00:00:00:00");
    ui->downTimerLabel->hide();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete mainTimer;
    delete downTimer;
}


void MainWindow::on_startButton_clicked()
{
    if (!mainTimer->isActive())
    {
        mainTimer->start();

        if (isMainTimerStoped)
        {
            mainCounter = 0;
            isMainTimerStoped = false;
        }
    }
}


void MainWindow::on_pauseButton_clicked()
{
    if (mainTimer->isActive())
        mainTimer->stop();
    else if (!isMainTimerStoped)
        mainTimer->start();
}


void MainWindow::on_stopButton_clicked()
{
    if (!isMainTimerStoped)
    {
        mainTimer->stop();
        isMainTimerStoped = true;
    }
}


void MainWindow::on_armagedonButton_clicked()
{
    downCounter = 60*100;
    downTimer->start();
    ui->armagedonButton->setEnabled(false);
    ui->downTimerLabel->show();
}


void MainWindow::onMainTimerTimeout()
{
    mainCounter++;
    ui->mainTimerLabel->display(formatNumberAsTime(mainCounter));
}

void MainWindow::onDownTimerTimeout()
{
    downCounter--;
    if (downCounter > 0)
    {
        ui->downTimerLabel->display(formatNumberAsTime(downCounter));
    }
    else
    {
        downTimer->stop();
        ui->armagedonButton->setEnabled(true);
        ui->downTimerLabel->hide();
    }
}

QString MainWindow::formatNumberAsTime(int number)
{
    int fractionsOfSeconds{number % 100};
    int seconds{number / 100};
    int minutes{seconds / 60};
    int hours{minutes / 60};
    seconds %= 60;
    minutes %= 60;

    return QString("%1:%2:%3:%4").arg(formatDigit(hours),
                                      formatDigit(minutes),
                                      formatDigit(seconds),
                                      formatDigit(fractionsOfSeconds));
}

QString MainWindow::formatDigit(int digit)
{
    if (digit < 10)
        return "0" + QString::number(digit);
    else
        return QString::number(digit);
}
