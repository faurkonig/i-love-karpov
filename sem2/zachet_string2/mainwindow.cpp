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

/// Сравниваем строки
void MainWindow::on_pushButtonResult_clicked()
{
    comp.setStr1(ui->lineEditStr1->text());
    comp.setStr2(ui->lineEditStr2->text());

    if (comp.compare())
        ui->labelResult->setText("Строки одинаковые");
    else
        ui->labelResult->setText("Строки разные");
}
