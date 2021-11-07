#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "serverwindow.h"
#include "clientwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButtonServer_clicked()
{
    auto window = new ServerWindow(this);
    window->show(); // Создание и открытие окна сервера
    close(); // Закрытие текущего окна
}


void MainWindow::on_pushButtonClient_clicked()
{
    auto window = new ClientWindow(this);
    window->show(); // Создание и открытие окна клиента
    close(); // Закрытие текущего окна
}

