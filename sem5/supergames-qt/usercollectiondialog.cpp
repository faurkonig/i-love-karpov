#include "usercollectiondialog.h"
#include "ui_usercollectiondialog.h"

UserCollectionDialog::UserCollectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserCollectionDialog)
{
    ui->setupUi(this);
}

UserCollectionDialog::~UserCollectionDialog()
{
    delete ui;
}

void UserCollectionDialog::on_sideList_currentRowChanged(int currentRow)
{

}
