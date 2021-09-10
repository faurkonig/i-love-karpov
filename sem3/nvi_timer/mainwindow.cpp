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

    additionalTimer = new QTimer(this);
    connect(additionalTimer, &QTimer::timeout, this, &MainWindow::onAdditionalTimerTimeout);
    additionalTimer->setInterval(10);
    ui->additionalTimerLabel->display("00:00:00:00");
    ui->additionalTimerLabel->hide();
}

MainWindow::~MainWindow()
{
    delete ui;

    delete mainTimer;
    delete downTimer;
    delete additionalTimer;
}


void MainWindow::on_startButton_clicked()
{
    if (!mainTimer->isActive())
    {
        mainTimer->start();

        if (isMainTimerStoped)
        {
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
        mainCounter = 0;
    }
}

void MainWindow::on_armagedonButton_clicked()
{
    downCounter = 60*100;
    downTimer->start();
    ui->armagedonButton->setEnabled(false);
    ui->downTimerLabel->show();
}

void MainWindow::on_additionaTimerButton_clicked()
{
    additionalCounter = ui->additionalTimerSpinBox1->value()*100;
    additionalTimer->start();
    ui->spinBoxWidget->setEnabled(false);
    ui->additionaTimerButton->setEnabled(false);
    ui->additionalTimerLabel->show();
}


void MainWindow::onMainTimerTimeout()
{
    mainCounter++;
    ui->mainTimerLabel->display(formatNumberAsTime(mainCounter));
}

void MainWindow::onDownTimerTimeout()
{
    downCounter--;
    if (downCounter <= 0)
    {
        downTimer->stop();
        mainTimer->stop();
        additionalTimer->stop();
        ui->centralwidget->setEnabled(false);
    }
    ui->downTimerLabel->display(formatNumberAsTime(downCounter));
}

void MainWindow::onAdditionalTimerTimeout()
{
    additionalCounter--;
    if (additionalCounter <= 0)
    {
        if (!isAdditionalTriggered)
        {
            isAdditionalTriggered = true;
            additionalCounter = ui->additionalTimerSpinBox2->value()*100;
            ui->startButton->click();
            ui->startButton->setEnabled(false);
            ui->stopButton->setEnabled(false);
        }
        else
        {
            isAdditionalTriggered = false;
            additionalTimer->stop();
            ui->startButton->setEnabled(true);
            ui->stopButton->setEnabled(true);
            ui->stopButton->click();
            ui->additionaTimerButton->setEnabled(true);
            ui->spinBoxWidget->setEnabled(true);
        }
    }
    ui->additionalTimerLabel->display(formatNumberAsTime(additionalCounter));
}


QString MainWindow::formatNumberAsTime(int number)
{
    if (number < 0)
        number = 0;

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
