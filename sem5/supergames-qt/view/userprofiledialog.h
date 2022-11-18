#ifndef USERPROFILEDIALOG_H
#define USERPROFILEDIALOG_H

#include "utils/databasecontainer.h"
#include "components/frienditem.h"
#include <QDialog>
#include <QList>


namespace Ui {
class UserProfileDialog;
}

class UserProfileDialog : public QDialog, private DatabaseContainer
{
    Q_OBJECT

public:
    /// Конструктор
    explicit UserProfileDialog(int currentUser, int userToSearch, QSqlDatabase *newDb, QWidget *parent = nullptr);
    /// Деструктор
    ~UserProfileDialog();

private slots:
    /// Обработчик нажатия на кнопку добавления в друзья
    void on_addByLoginButton_clicked();

    /// Обработчик нажатия на кнопку удаления из друзей
    void deleteFriend(int friendshipId);

private:
    Ui::UserProfileDialog *ui;

    /// ID текущего пользователя, от имени которого мы запрашиваем профиль
    int currentUserId;
    /// ID пользователя, профиль которого мы хотим показать
    int profileUserId;
    // Разделение этих двух ID нужен был для того, чтобы реализовать
    // возможность смотреть профили других пользователей

    /// SQL запрос для поулчения всей необходимой информации о пользователе
    QString userQuerySql{
        "SELECT"
        " u.login, u.\"name\", u.\"date\","
        " (SELECT count(id) AS friends_count"
        " FROM internal.friendship f"
        " WHERE f.\"source\" = u.id) "
        "FROM users u "
        "WHERE u.id = %1"
    };
    /// SQL запрос для получения списка друзей пользователя
    QString friendsQuerySql{
        "SELECT id,"
        " (SELECT \"name\" FROM public.users u"
        " WHERE u.id = f.target) "
        "FROM internal.friendship f "
        "WHERE f.\"source\" = %1"
    };
    /// SQL запрос для удаления пользователя из друзей
    QString removeFriendQuerySql{
        "DELETE FROM internal.friendship f "
        "WHERE f.id = %1 "
        "RETURNING (SELECT u.\"name\" FROM public.users u"
        " WHERE u.id = f.target)"
    };

    /// Список виджета "друга"
    QList<FriendItem *> friendItems;

    /// Метод для обновления профиля
    void updateProfile();
};

#endif // USERPROFILEDIALOG_H
