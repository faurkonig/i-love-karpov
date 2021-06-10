#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->labelRes->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    int res = 1;
    for (int i = 12; i > 1; i--)
    {
        res *= i;
    }
    ui->labelRes->setNum(res);
    ui->labelRes->show();
}
