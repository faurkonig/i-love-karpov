#include "gamedialog.h"
#include "commonpatterns.h"
#include "ui_gamedialog.h"
#include "dialoghelper.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDateTime>

GameDialog::GameDialog(QSqlDatabase *newDb, int id, SgUser user, QWidget *parent) :
    QDialog(parent),
    DatabaseContainer(parent, newDb),
    ui(new Ui::GameDialog),
    gameId(id),
    user(user)
{
    ui->setupUi(this);

    updateData();

    if (user.isGuest()) {
        ui->buyButton->setEnabled(false);
    }
}

GameDialog::~GameDialog()
{
    delete ui;
}

void GameDialog::updateData()
{
    if (!checkDatabase()) return;

    auto gameQ = mainDatabase->exec(gameQuerySql.arg(gameId));

    if (DialogHelper::isSqlError(gameQ.lastError())) {
        DialogHelper::showSqlError(this, gameQ.lastError(), gameQuerySql);
        return;
    }

    gameQ.first();

    auto gameName = gameQ.value(0).toString();
    auto gameDescription = gameQ.value(1).toString();
    auto gamePrice = gameQ.value(2).toDouble();
    auto gameDate = gameQ.value(3).toDateTime();
    auto devName = gameQ.value(4).toString();
    auto devDescription = gameQ.value(5).toString();
    auto devEmail = gameQ.value(6).toString();
    auto devDate = gameQ.value(7).toDateTime();
    auto reviewCount = gameQ.value(8).toInt();
    auto collectionCount = gameQ.value(9).toInt();

    ui->gameTitle->setText(gameName);
    ui->gameDescription->setText(gameDescription);
    ui->gameDeveloper->setText(QString("Разработана <u>%1</u>")
                               .arg(devName));
    ui->gameDate->setText(QString("Добавлена <u>%1</u>")
                          .arg(gameDate.toLocalTime().toString(CommonPatterns::dateTimeFormat)));
    ui->gameCollectionCount->setText(QString("В коллекции у <u>%1</u> пользователей")
                                     .arg(collectionCount));
    ui->developerDescription->setText(devDescription);
    ui->developerDate->setText(QString("Зарегистрирован <u>%1</u>")
                               .arg(devDate.toLocalTime().toString(CommonPatterns::dateTimeFormat)));
    ui->developerEmail->setText(devEmail);

    if (gamePrice > 0) {
        ui->buyButton->setText(QString("Добавить \"%1\" в корзину (%2 руб.)")
                               .arg(gameName).arg(gamePrice));
    } else {
        ui->buyButton->setText(QString("Добавить \"%1\" в коллекцию (Бесплатно)")
                               .arg(gameName));
    }

    if (reviewCount > 0) {
        ui->noReviewLabel->hide();
    }
}
