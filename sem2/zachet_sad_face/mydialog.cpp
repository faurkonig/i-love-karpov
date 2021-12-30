#include "mydialog.h"
#include "ui_mydialog.h"

MyDialog::MyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MyDialog)
{
    ui->setupUi(this);
}

MyDialog::~MyDialog()
{
    delete ui;
}

QString MyDialog::lineEditValue()
{
    return ui->lineEdit->text();
}

void MyDialog::on_pushButton_clicked()
{
    close();
}
