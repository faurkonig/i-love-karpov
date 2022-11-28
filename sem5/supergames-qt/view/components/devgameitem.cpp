#include "devgameitem.h"
#include "ui_devgameitem.h"

DevGameItem::DevGameItem(int id, QString name, QString description, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DevGameItem),
    gameId(id)
{
    ui->setupUi(this);

    ui->titleLabel->setText(name);
    ui->descriptionLabel->setText(description);
}

DevGameItem::~DevGameItem()
{
    delete ui;
}


void DevGameItem::on_openButton_clicked()
{
    emit onOpenGameButtonPressed(gameId);
}

void DevGameItem::on_editButton_clicked()
{
    emit onEditGameButtonPressed(gameId);
}

