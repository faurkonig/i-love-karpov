#include "mainwindow.h"
#include "ui_mainwindow.h"

/// Конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/// Деструктор
MainWindow::~MainWindow()
{
    delete ui;
}

/// Добавление значения в класс
void MainWindow::on_pushButtonAdd_clicked()
{
    bool ok;
    int value = ui->lineEditValue->text().toInt(&ok);

    if (ok)
    {
        ui->labelResult->setText("");
        array.addValue(value);
        ui->labelCount->setText("Добавлено значений: "+QString::number(array.length()));
    }
    else
    {
        ui->labelResult->setText("Неверное значение");
    }
}

/// Нахождение суммы отрицательных значений
void MainWindow::on_pushButtonResult_clicked()
{
    if (array.length() > 0)
    {
        int result = array.negativeSum();
        if (result < 0)
        {
            ui->labelResult->setNum(result);
        }
        else
        {
            ui->labelResult->setText("Сумма слишком маленькая");
        }
    }
    else
    {
        ui->labelResult->setText("Нет значений");
    }
}

/// Очистка массива значений
void MainWindow::on_pushButtonClear_clicked()
{
    array.clearAll();
    ui->labelCount->setText("Добавлено значений: 0");
}

