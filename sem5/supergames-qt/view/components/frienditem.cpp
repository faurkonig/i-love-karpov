#include "frienditem.h"
#include "ui_frienditem.h"

FriendItem::FriendItem(int friendship, QString userName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FriendItem),
    friendshipId(friendship)
{
    ui->setupUi(this);

    ui->nameLabel->setText(userName);
}

FriendItem::~FriendItem()
{
    delete ui;
}

void FriendItem::on_deleteButton_clicked()
{
    emit onDeleteButtonPressed(friendshipId);
}
