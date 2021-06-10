#include "mywindow.h"
#include "ui_mywindow.h"

MyWindow::MyWindow(QWidget *parent, QLabel *l) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    ui->setupUi(this);

    label = l;
}

MyWindow::~MyWindow()
{
    delete ui;
}

void MyWindow::on_pushButton_clicked()
{
    label->setText(ui->lineEdit->text());
    close();
}
