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

/// Высчитывание факториала 12 и вывод результата на экран
void MainWindow::on_pushButton_clicked()
{
    int res = 1;
    for (int i = 12; i > 1; i--)
        res *= i;

    ui->labelRes->setNum(res);
}
