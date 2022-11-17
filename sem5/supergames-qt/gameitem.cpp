#include "gameitem.h"
#include "ui_gameitem.h"

GameItem::GameItem(int id, QString name, QString description, QString developer, double price, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::GameItem),
      id(id)
{
    ui->setupUi(this);

    // Обновляем виджеты по заданным в конструкторе данным
    ui->gameTitle->setText(name);
    ui->gameDescription->setText(description);
    ui->gameDeveloper->setText(developer);
    if (price > 0) {
        ui->priceLabel->setText(QString::number(price) + " руб.");
    } else {
        ui->priceLabel->setText("Бесплатно");
    }
}

GameItem::~GameItem()
{
    delete ui;
}

void GameItem::on_buyButton_clicked()
{
    emit onGameButtonPressed(id);
}
