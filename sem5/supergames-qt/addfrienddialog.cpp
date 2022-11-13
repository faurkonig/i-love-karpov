#include "addfrienddialog.h"
#include "ui_addfrienddialog.h"
#include "commonpatterns.h"
#include "dialoghelper.h"
#include <QtSql/QSqlQuery>

AddFriendDialog::AddFriendDialog(QSqlDatabase *newDb, int currentUser, QWidget *parent) :
    QDialog(parent),
    DatabaseContainer(this, newDb),
    ui(new Ui::AddFriendDialog),
    sourceId(currentUser)
{
    ui->setupUi(this);

    ui->infoLabel->hide();
}

AddFriendDialog::~AddFriendDialog()
{
    delete ui;
}

void AddFriendDialog::on_addButton_clicked()
{
    auto loginToAdd = ui->loginField->text().trimmed().toLower();
    if (!loginToAdd.contains(CommonPatterns::loginRegex)) {
        showError("Неверный логин");
        return;
    }

    if (!checkDatabase()) {
        showError("Нет подключения к базе данных");
        return;
    }

    auto findByLoginQuery = QString("SELECT id, \"name\" FROM users WHERE login = '%1'").arg(loginToAdd);
    auto foundUser = mainDatabase->exec(findByLoginQuery);
    if (DialogHelper::isSqlError(foundUser.lastError())) {
        DialogHelper::showSqlError(this, foundUser.lastError(), findByLoginQuery);
        return;
    }
    if (foundUser.size() < 1) {
        showError("Пользователь с таким логином не найден");
        return;
    }
    foundUser.first();
    auto userId = foundUser.value(0).toInt();
    auto userName = foundUser.value(1).toString();

    if (userId == sourceId) {
        showError("Нельзя добавить в друзья самого себя");
        return;
    }

    auto findFriendshipQuery = QString("SELECT id FROM internal.friendship "
                                       "WHERE \"source\" = %1 AND \"target\" = %2")
            .arg(sourceId).arg(userId);
    auto foundFriendship = mainDatabase->exec(findFriendshipQuery);
    if (DialogHelper::isSqlError(foundFriendship.lastError())) {
        DialogHelper::showSqlError(this, foundFriendship.lastError(), findFriendshipQuery);
        return;
    }
    if (foundFriendship.size() != 0) {
        showError("Этот пользователь уже у вас в друзьях");
        return;
    }

    auto addFriendQuery = QString("INSERT INTO internal.friendship (\"source\", \"target\") "
                                  "VALUES (%1, %2)").arg(sourceId).arg(userId);
    auto insertedFriendship = mainDatabase->exec(addFriendQuery);
    if (DialogHelper::isSqlError(insertedFriendship.lastError())) {
        DialogHelper::showSqlError(this, insertedFriendship.lastError(), addFriendQuery);
        return;
    }

    showSuccess("Пользователь \""+userName+"\" добавлен в друзья!");
}


void AddFriendDialog::on_exitButton_clicked()
{
    accept();
}

void AddFriendDialog::showError(QString text)
{
    ui->infoLabel->setText("<font color=\"red\">"+text+"</font>");
    ui->infoLabel->show();
}

void AddFriendDialog::showSuccess(QString text)
{
    ui->infoLabel->setText("<font color=\"green\">"+text+"</font>");
    ui->infoLabel->show();
}
