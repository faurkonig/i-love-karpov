#include "userprofiledialog.h"
#include "utils/commonpatterns.h"
#include "ui_userprofiledialog.h"
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
    bool ok;
    auto q = execQuery(userQuerySql.arg(profileUserId), ok);
    if (!ok) return;
    q.first();

    // Берём результаты запросы
    auto login = q.value(0).toString();
    auto userName = q.value(1).toString();
    auto userDate = q.value(2).toDateTime();
    auto friendsCount = q.value(3).toInt();

    // Выводим основные данные о пользователе
    ui->nameTitle->setText(userName);
    ui->loginLabel->setText(QString("Логин: <b>%1</b>").arg(login));
    ui->dateLabel->setText(QString("Зарегистрирован <u>%1</u>")
                           .arg(userDate.toLocalTime()
                                .toString(CommonPatterns::dateTimeFormat)));

    // Очищаем список друзей
    for (auto fi : qAsConst(friendItems)) {
        ui->friendsLayout->removeWidget(fi);
        fi->deleteLater();
    }
    friendItems.clear();

    // Проверяем, надо ли показывать друзей
    if (friendsCount < 1) return;
    ui->noFriendsLabel->hide();

    auto friends = execQuery(friendsQuerySql.arg(profileUserId), ok);
    if (!ok) return;

    while(friends.next()) {
        // Выводим загруженных друзей
        auto fi = new FriendItem(friends.value(0).toInt(), friends.value(1).toString(),
                                 ui->friendsBox);
        friendItems.append(fi);
        connect(fi, &FriendItem::onDeleteButtonPressed, this, &UserProfileDialog::deleteFriend);
        ui->friendsLayout->addWidget(fi);
    }
}

void UserProfileDialog::deleteFriend(int friendshipId)
{
    bool ok;
    // Удаляем друга
    auto q = execQuery(removeFriendQuerySql.arg(friendshipId), ok);
    if (!ok) return;

    // Получаем элемент, который вызвал слот и удаляем его
    auto fi = static_cast<FriendItem *>(sender());
    ui->friendsLayout->removeWidget(fi);
    friendItems.removeOne(fi);
    fi->deleteLater();

    if (q.size() > 0) {
        // Если в результате запроса об удалении нам вернулся этот пользователь,
        // то показываем диалог о том, что пользователь был удалён из друзей
        q.first();
        QMessageBox::information(this, "Дружба",
                                 QString("Вы больше не друзья с \"%1\" ;(")
                                 .arg(q.value(0).toString()));
    }

    // Показываем текст о том, что пользователя нет друзей, если мы только что удалили последнего
    if (friendItems.isEmpty()) {
        ui->noFriendsLabel->show();
    }
}

void UserProfileDialog::on_addByLoginButton_clicked()
{
    AddFriendDialog(mainDatabase, currentUserId, this).exec();
    updateProfile();
}
