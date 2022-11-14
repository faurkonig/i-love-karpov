#include "usercartdialog.h"
#include "ui_usercartdialog.h"
#include "gameitem.h"
#include "gamedialog.h"
#include <QMessageBox>

UserCartDialog::UserCartDialog(int currentUser, QSqlDatabase *newDb, QWidget *parent) :
    QDialog(parent),
    DatabaseContainer(parent, newDb),
    ui(new Ui::UserCartDialog),
    userId(currentUser)
{
    ui->setupUi(this);

    updateContent();
}

UserCartDialog::~UserCartDialog()
{
    delete ui;
}

void UserCartDialog::updateContent()
{
    bool ok;
    auto q = execQuery(gamesInCartSqlQuery.arg(userId), ok);
    if (!ok) return;

    for (auto ci : qAsConst(cartItems)) {
        ui->scrollVerticalLayout->removeWidget(ci);
        ci->deleteLater();
    }
    cartItems.clear();

    if (q.size() > 0) {
        ui->noItemsLabel->hide();

        while (q.next()) {
            auto cartItem = new QWidget(ui->scrollArea);
            auto cartItemLayout = new QVBoxLayout(cartItem);
            ui->scrollVerticalLayout->insertWidget(1, cartItem);
            cartItems.append(cartItem);

            auto gameId = q.value(1);
            auto gameName = q.value(2);
            auto gameDescription = q.value(3).toString();
            auto gamePrice = q.value(4);
            auto gameDeveloper = q.value(5);

            auto gi = new GameItem(gameId.toInt(), gameName.toString(),
                                   gameDescription, gameDeveloper.toString(),
                                   gamePrice.toInt(), cartItem);
            connect(gi, &GameItem::onGameButtonPressed, this, &UserCartDialog::openGame);
            cartItemLayout->addWidget(gi);

            auto pb = new QPushButton("Купить", cartItem);
            connect(pb, &QPushButton::clicked, this, &UserCartDialog::buyGame);
            pb->setProperty("cartId", q.value(0));
            pb->setProperty("gameId", gameId);
            pb->setProperty("name", gameName);
            pb->setProperty("price", gamePrice);
            pb->setProperty("developer", gameDeveloper);
            cartItemLayout->addWidget(pb);
        }
    } else {
        ui->noItemsLabel->show();
    }
}


void UserCartDialog::on_cancelButton_clicked()
{
    reject();
}

void UserCartDialog::openGame(int gameId)
{
    GameDialog(mainDatabase, gameId, userId, this).exec();
}

void UserCartDialog::buyGame()
{
    auto source = static_cast<QPushButton *>(sender());
    auto cartId = source->property("cartId").toInt();
    auto gameId = source->property("gameId").toInt();
    auto name = source->property("name").toString();
    auto price = source->property("price").toDouble();
    auto developer = source->property("developer").toString();

    auto question = QMessageBox::question(this, "Покупка",
                                          QString("Вы уверены что хотите купить игру \"%1\" "
                                                  "от разработчика \"%2\" "
                                                  "за %3 рублей?")
                                          .arg(name, developer).arg(price),
                                          QMessageBox:: Yes | QMessageBox::Cancel);
    if (question != QMessageBox::Yes) return;

    bool ok;
    // Удаляем игру из корзины
    execQuery(QString("DELETE FROM internal.cart_elements "
                      "WHERE id = %1").arg(cartId), ok);
    if (!ok) return;

    // Добавляем игру в коллекцию
    execQuery(QString("INSERT INTO internal.collection_elements (game, \"user\")"
                      "VALUES (%1, %2)").arg(gameId).arg(userId), ok);
    if (!ok) return;

    updateContent();
}