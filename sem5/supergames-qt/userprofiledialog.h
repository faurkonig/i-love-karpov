#ifndef USERPROFILEDIALOG_H
#define USERPROFILEDIALOG_H

#include "databasecontainer.h"
#include "frienditem.h"
#include <QDialog>
#include <QList>

namespace Ui {
class UserProfileDialog;
}

class UserProfileDialog : public QDialog, private DatabaseContainer
{
    Q_OBJECT

public:
    explicit UserProfileDialog(int currentUser, int userToSearch, QSqlDatabase *newDb, QWidget *parent = nullptr);
    ~UserProfileDialog();

private slots:
    void on_addByLoginButton_clicked();

    void deleteFriend(int friendshipId);

private:
    Ui::UserProfileDialog *ui;

    int currentUserId;
    int profileUserId;

    QString userQuerySql{
        "SELECT"
        " u.login, u.\"name\", u.\"date\","
        " (SELECT count(id) AS friends_count"
        " FROM internal.friendship f"
        " WHERE f.\"source\" = u.id) "
        "FROM users u "
        "WHERE u.id = %1"
    };
    QString friendsQuerySql{
        "SELECT id,"
        " (SELECT \"name\" FROM public.users u"
        " WHERE u.id = f.target) "
        "FROM internal.friendship f "
        "WHERE f.\"source\" = %1"
    };
    QString removeFriendQuerySql{
        "DELETE FROM internal.friendship f "
        "WHERE f.id = %1 "
        "RETURNING (SELECT u.\"name\" FROM public.users u"
        " WHERE u.id = f.target)"
    };

    QList<FriendItem *> friendItems;

    void updateProfile();
};

#endif // USERPROFILEDIALOG_H
