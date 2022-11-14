#ifndef USERCARTDIALOG_H
#define USERCARTDIALOG_H

#include "databasecontainer.h"
#include <QDialog>

namespace Ui {
class UserCartDialog;
}

class UserCartDialog : public QDialog, private DatabaseContainer
{
    Q_OBJECT

public:
    explicit UserCartDialog(int currentUser, QSqlDatabase *newDb, QWidget *parent = nullptr);
    ~UserCartDialog();

private slots:
    void on_cancelButton_clicked();

    void openGame(int gameId);

    void buyGame();

private:
    Ui::UserCartDialog *ui;

    int userId;

    QString gamesInCartSqlQuery{
        "SELECT ce.id AS cart_id, g.id AS game_id,"
        " g.\"name\", g.description, g.price,"
        " (SELECT d.\"name\" FROM public.developers d"
        " WHERE d.id = g.developer) "
        "FROM internal.cart_elements ce "
        "JOIN (SELECT"
        " id, \"name\", description, price, developer"
        " FROM public.games) AS g"
        " ON g.id = ce.game "
        "WHERE ce.\"user\" = %1"
    };

    QList<QWidget *> cartItems;

    void updateContent();
};

#endif // USERCARTDIALOG_H
