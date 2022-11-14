#ifndef USERCOLLECTIONDIALOG_H
#define USERCOLLECTIONDIALOG_H


#include "databasecontainer.h"
#include <QDialog>

namespace Ui {
class UserCollectionDialog;
}

class UserCollectionDialog : public QDialog, private DatabaseContainer
{
    Q_OBJECT

public:
    explicit UserCollectionDialog(int currentUser, QSqlDatabase *newDb, QWidget *parent = nullptr);
    ~UserCollectionDialog();

private slots:
    void on_sideList_currentRowChanged(int currentRow);

private:
    Ui::UserCollectionDialog *ui;

    int userId;

    QString collectionListSqlQuery{
        "SELECT g.id, g.\"name\" FROM public.games g "
        "WHERE g.id IN (SELECT ce.game"
        " FROM internal.collection_elements ce"
        " WHERE ce.\"user\" = %1) "
    };

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

    QList<int> gameIds;

    void updateSideList();
};

#endif // USERCOLLECTIONDIALOG_H
