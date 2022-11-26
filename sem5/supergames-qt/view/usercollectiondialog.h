#ifndef USERCOLLECTIONDIALOG_H
#define USERCOLLECTIONDIALOG_H

#include "utils/databasecontainer.h"
#include <QDialog>


namespace Ui {
class UserCollectionDialog;
}

class UserCollectionDialog : public QDialog, private DatabaseContainer
{
    Q_OBJECT

public:
    /// Конструктор
    explicit UserCollectionDialog(int currentUser, QSqlDatabase *newDb, QWidget *parent = nullptr);
    /// Деструктор
    ~UserCollectionDialog();

private slots:
    /// Обработчик выбора игры в боковой панеле
    void on_sideList_currentRowChanged(int currentRow);

    void on_reviewButton_clicked();

private:
    Ui::UserCollectionDialog *ui;

    int userId;

    /// SQL запрос для получения списка игр в коллекции
    QString collectionListSqlQuery{
        "SELECT g.id, g.\"name\" FROM public.games g "
        "WHERE g.id IN (SELECT ce.game"
        " FROM internal.collection_elements ce"
        " WHERE ce.\"user\" = %1) "
    };

    /// SQL запрос для получения информации о конкретной игре
    QString gameSqlQuery{
        "SELECT g.\"name\", g.description,"
        " (SELECT d.\"name\" AS dev_name  FROM public.developers d"
        "  WHERE d.id = g.developer),"
        " ce.date "
        "FROM internal.collection_elements ce "
        "JOIN (SELECT id, \"name\", description, developer"
        " FROM public.games) AS g"
        " ON g.id = ce.game "
        "WHERE ce.game = %1 AND ce.\"user\" = %2"
    };

    /// Список ID игр, который соотнесён с элементами в боковом меню
    QList<int> gameIds;

    /// Метод для обновления списка в боковом меню
    void updateSideList();
};

#endif // USERCOLLECTIONDIALOG_H
