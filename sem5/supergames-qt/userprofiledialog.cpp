#include "userprofiledialog.h"
#include "commonpatterns.h"
#include "ui_userprofiledialog.h"
#include "dialoghelper.h"
#include "addfrienddialog.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDateTime>
#include <QMessageBox>

UserProfileDialog::UserProfileDialog(int currentUser, int userToSearch, QSqlDatabase *newDb, QWidget *parent) :
    QDialog(parent),
    DatabaseContainer(parent, newDb),
    ui(new Ui::UserProfileDialog),
    currentUserId(currentUser),
    profileUserId(userToSearch)
{
    ui->setupUi(this);

    updateProfile();
}

UserProfileDialog::~UserProfileDialog()
{
    delete ui;
}

void UserProfileDialog::updateProfile()
{
    if (!checkDatabase()) return;

    auto query = userQuerySql.arg(profileUserId);
    auto q = mainDatabase->exec(query);
    q.first();
    if (DialogHelper::isSqlError(q.lastError())) {
        DialogHelper::showSqlError(this, q.lastError(), query);
        return;
    }

    auto login = q.value(0).toString();
    auto userName = q.value(1).toString();
    auto userDate = q.value(2).toDateTime();
    auto friendsCount = q.value(3).toInt();

    ui->nameTitle->setText(userName);
    ui->loginLabel->setText(QString("Логин: <b>%1</b>").arg(login));
    ui->dateLabel->setText(QString("Зарегистрирован <u>%1</u>")
                           .arg(userDate.toLocalTime().toString(CommonPatterns::dateTimeFormat)));

    for (auto fi : qAsConst(friendItems)) {
        ui->friendsLayout->removeWidget(fi);
        fi->deleteLater();
    }
    friendItems.clear();

    if (friendsCount > 0) {
        ui->noFriendsLabel->hide();

        auto friendsQuery = friendsQuerySql.arg(profileUserId);
        auto friends = mainDatabase->exec(friendsQuery);
        if (DialogHelper::isSqlError(friends.lastError())) {
            DialogHelper::showSqlError(this, friends.lastError(), friendsQuery);
            return;
        }

        while(friends.next()) {
            auto fi = new FriendItem(friends.value(0).toInt(), friends.value(1).toString(),
                                     ui->friendsBox);
            friendItems.append(fi);
            connect(fi, &FriendItem::onDeleteButtonPressed, this, &UserProfileDialog::deleteFriend);
            ui->friendsLayout->addWidget(fi);
        }
    }
}

void UserProfileDialog::deleteFriend(int friendshipId)
{
    auto query = removeFriendQuerySql.arg(friendshipId);
    auto q = mainDatabase->exec(query);
    if (DialogHelper::isSqlError(q.lastError())) {
        DialogHelper::showSqlError(this, q.lastError(), query);
        return;
    }

    if (q.size() > 0) {
        q.first();
        QMessageBox::information(this, "Дружба",
                                 QString("Вы больше не друзья с \"%1\" ;(")
                                 .arg(q.value(0).toString()));
    }

    updateProfile();
}

void UserProfileDialog::on_addByLoginButton_clicked()
{
    AddFriendDialog(mainDatabase, currentUserId, this).exec();
    updateProfile();
}
