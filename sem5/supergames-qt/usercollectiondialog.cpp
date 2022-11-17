#include "usercollectiondialog.h"
#include "ui_usercollectiondialog.h"
#include "commonpatterns.h"
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
    auto q = execQuery(collectionListSqlQuery.arg(userId), ok);
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
    auto q = execQuery(gameSqlQuery.arg(gameIds[currentRow]).arg(userId), ok);
    if (!ok || q.size() < 1) return;

    // Получаем и выводим данные об выбранной игре
    q.first();
    ui->gameTitle->setText(q.value(0).toString());
    ui->descriptionLabel->setText(q.value(1).toString());
    ui->devNameLabel->setText(QString("Разработана <u>%1</u>")
                              .arg(q.value(2).toString()));
    ui->dateLabel->setText(QString("Куплена <u>%1</u>")
                           .arg(q.value(3).toDateTime()
                                .toLocalTime().toString(CommonPatterns::dateTimeFormat)));

    ui->noSelectionLabel->hide();
    ui->gameBlock->show();
}
