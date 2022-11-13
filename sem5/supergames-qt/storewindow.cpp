#include "storewindow.h"
#include "ui_storewindow.h"
#include "dialoghelper.h"
#include "gameitem.h"
#include "gamedialog.h"
#include "userprofiledialog.h"

StoreWindow::StoreWindow(SgUser user, QSqlDatabase *newDb, QWidget *parent) :
    QMainWindow(parent),
    DatabaseContainer(parent, newDb),
    ui(new Ui::StoreWindow),
    user(user)
{
    ui->setupUi(this);

    ui->profileLabel->setText(QString("Вы вошли как \"%1\"").arg(user.name));
    if (user.isGuest()) {
        ui->profileButton->setEnabled(false);
        ui->collectionButton->setEnabled(false);
        ui->cartButton->setEnabled(false);
    }

    updateGames();
}

StoreWindow::~StoreWindow()
{
    delete ui;
}

void StoreWindow::on_updateButton_clicked()
{
    updateGames();
}

void StoreWindow::updateGames()
{
    if (!checkDatabase()) return;

    auto getGamesQ = mainDatabase->exec(gamesQuerySql);
    if (DialogHelper::isSqlError(getGamesQ.lastError())) {
        DialogHelper::showSqlError(this, getGamesQ.lastError(), gamesQuerySql);
        return;
    }

    for (auto gi : qAsConst(gameItems)) {
        ui->scrollVerticalLayout->removeWidget(gi);
        gi->deleteLater();
    }
    gameItems.clear();

    int id;
    QString name;
    QString description;
    double price;
    QString developerName;
    GameItem *gi;
    while (getGamesQ.next()) {
        id = getGamesQ.value(0).toInt();
        name = getGamesQ.value(1).toString();
        description = getGamesQ.value(2).toString();
        price = getGamesQ.value(3).toDouble();
        developerName = getGamesQ.value(4).toString();

        gi = new GameItem(id, name, description, developerName, price, ui->scrollAreaWidgetContents);
        connect(gi, &GameItem::onGameButtonPressed, this, &StoreWindow::openGame);
        gameItems.append(gi);
        ui->scrollVerticalLayout->insertWidget(1, gi);
    }

    ui->totalCountLabel->setText("Загружено игр: " + QString::number(gameItems.length()));
}

void StoreWindow::openGame(int gameId)
{
    auto gameDialog = GameDialog(mainDatabase, gameId, user, this);
    gameDialog.exec();
}


void StoreWindow::on_profileButton_clicked()
{
    UserProfileDialog(user.id, user.id, mainDatabase, this).exec();
}

