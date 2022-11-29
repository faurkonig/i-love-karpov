#include "usercollectiondialog.h"
#include "ui_usercollectiondialog.h"
#include "utils/dialoghelper.h"
#include "reviewdialog.h"
#include "gamedialog.h"
#include <QDateTime>

UserCollectionDialog::UserCollectionDialog(int currentUser, QSqlDatabase *newDb, QWidget *parent) :
    QDialog(parent),
    DatabaseContainer(parent, newDb),
    ui(new Ui::UserCollectionDialog),
    userId(currentUser)
{
    ui->setupUi(this);

    ui->gameBlock->hide();

    updateSideList();
}

UserCollectionDialog::~UserCollectionDialog()
{
    delete ui;
}


void UserCollectionDialog::updateSideList()
{
    bool ok;
    // Грузим игры для боковой панели из БД
    auto q = execQuery(QString("SELECT g.id, g.\"name\" FROM public.games g "
                               "WHERE g.id IN (SELECT ce.game"
                               " FROM internal.collection_elements ce"
                               " WHERE ce.\"user\" = %1) ")
                       .arg(userId), ok);
    if (!ok || q.size() < 1) {
        ui->noSelectionLabel->setText("Нет ни одной игры в коллекции");
        return;
    }

    while (q.next()) {
        // Выводи игры
        ui->sideList->addItem(q.value(1).toString());
        gameIds.append(q.value(0).toInt());
    }
}


void UserCollectionDialog::on_sideList_currentRowChanged(int currentRow)
{
    bool ok;
    auto q = execQuery(QString("SELECT g.\"name\", g.description,"
                               " (SELECT d.\"name\" AS dev_name  FROM public.developers d"
                               "  WHERE d.id = g.developer),"
                               " ce.date "
                               "FROM internal.collection_elements ce "
                               "JOIN (SELECT id, \"name\", description, developer"
                               " FROM public.games) AS g"
                               " ON g.id = ce.game "
                               "WHERE ce.game = %1 AND ce.\"user\" = %2")
                       .arg(gameIds[currentRow]).arg(userId), ok);
    if (!ok || q.size() < 1) return;

    // Получаем и выводим данные об выбранной игре
    q.first();
    ui->gameTitle->setText(q.value(0).toString());
    ui->descriptionLabel->setText(q.value(1).toString());
    ui->devNameLabel->setText(QString("Разработана <u>%1</u>")
                              .arg(q.value(2).toString()));
    ui->dateLabel->setText(QString("Куплена <u>%1</u>")
                           .arg(DialogHelper::formatTime(q.value(3).toDateTime())));

    ui->noSelectionLabel->hide();
    ui->gameBlock->show();

    updateReviewButton();
}

void UserCollectionDialog::updateReviewButton()
{
    bool ok;
    auto gameId = gameIds[ui->sideList->currentRow()];
    auto reviewQ = execQuery(QString("SELECT id FROM public.reviews "
                                     "WHERE \"user\" = %1 AND game = %2")
                             .arg(userId).arg(gameId), ok);

    if (!ok || reviewQ.size() < 1) {
        // Отзыва на эту игру нет
        ui->reviewButton->setText("Оставить отзыв");
    } else {
        ui->reviewButton->setText("Редактировать отзыв");
    }
}


void UserCollectionDialog::on_reviewButton_clicked()
{
    bool isChanged;
    // Вот эта запись в переменную и вызов по указателю были сделаны только для того,
    // чтобы избежать ошибки "*** stack smashing detected ***: terminated",
    // которая появилась после определения метода reject()
    auto d = new ReviewDialog(userId, gameIds[ui->sideList->currentRow()],
            &isChanged, mainDatabase, this);
    d->exec();

    if (isChanged) {
        updateReviewButton();
    }
}

void UserCollectionDialog::on_storeButton_clicked()
{
    GameDialog(mainDatabase, gameIds[ui->sideList->currentRow()],
            0, this).exec();
}
