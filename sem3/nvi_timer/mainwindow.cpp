#include "mainwindow.h"
#include "ui_mainwindow.h"

/// Конструктор
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

/// Деструктор
MainWindow::~MainWindow()
{
    delete ui;

    delete mainTimer;
    delete downTimer;
    delete additionalTimer;
}


/// Действие по нажатию кнопки Старт
void MainWindow::on_startButton_clicked()
{
    mainCounter = 0;
    mainTimer->start();
}

/// Действие по нажатию на паузу
void MainWindow::on_pauseButton_clicked()
{
    if (mainTimer->isActive())
        mainTimer->stop();
    else
        mainTimer->start();
}

/// Действие по нажатию кнопки Стоп
void MainWindow::on_stopButton_clicked()
{
    mainTimer->stop();
    isMainTimerStoped = true;
}

/// Действие по нажатию на кнопку армагедона
void MainWindow::on_armagedonButton_clicked()
{
    downCounter = 60*100;
    downTimer->start();
    ui->armagedonButton->setEnabled(false);
    ui->downTimerLabel->show();
}

/// Действие по нажатию на кнопку 3 таймера
void MainWindow::on_additionaTimerButton_clicked()
{
    additionalCounter = ui->additionalTimerSpinBox1->value()*100;
    additionalTimer->start();
    ui->spinBoxWidget->setEnabled(false);
    ui->additionaTimerButton->setEnabled(false);
    ui->additionalTimerLabel->show();
}


/// Действие по истечению главного (первого) таймера
void MainWindow::onMainTimerTimeout()
{
    mainCounter++;
    ui->mainTimerLabel->display(formatNumberAsTime(mainCounter));
}

/// Действие по истечению таймера армагедона
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

/// Действие по истечению третьего таймера
void MainWindow::onAdditionalTimerTimeout()
{
    additionalCounter--;
    if (additionalCounter <= 0)
    {
        if (!isAdditionalTriggered)
        {
            ui->startButton->click();
            isAdditionalTriggered = true;
            additionalCounter = ui->additionalTimerSpinBox2->value()*100;
        }
        else
        {
            isAdditionalTriggered = false;
            additionalTimer->stop();
            ui->additionaTimerButton->setEnabled(true);
            ui->spinBoxWidget->setEnabled(true);
            ui->stopButton->click();
        }
    }
    ui->additionalTimerLabel->display(formatNumberAsTime(additionalCounter));
}


/// Функция для форматирования числа как времени
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

/// Функция для форматирования одной цифры, чтобы она всегда записывалась с нулём
QString MainWindow::formatDigit(int digit)
{
    if (digit < 10)
        return "0" + QString::number(digit);
    else
        return QString::number(digit);
}
