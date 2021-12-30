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

/// Перевод числа в 10сс
void MainWindow::on_pushButtonConvert_clicked()
{
    // Считывание числа из поля (в виде 2сс)
    bool ok;
    int number = ui->lineEditNumber->text().toInt(&ok, 2);

    if (ok)
    {
        // Если введено правильное значение в 2сс, то выводим результат уже в 10сс
        ui->labelResult->setText(QString::number(number));
    }
    else
    {
        // Если введено неправильное значение, то выводим ошибку
        ui->labelResult->setText("Неверное значение");
    }
}
