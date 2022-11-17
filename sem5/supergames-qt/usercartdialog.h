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
    /// Конструктор
    explicit UserCartDialog(int currentUser, QSqlDatabase *newDb, QWidget *parent = nullptr);
    /// Деструктор
    ~UserCartDialog();

private slots:
    /// Обработчик нажатия по кнопке отмены
    void on_cancelButton_clicked();

    /// Обработчик нажатия на кнопку "Об игре" у конкретной игры
    void openGame(int gameId);

    /// Обработчик нажатия на кнопку "Купить" на элементе корзины
    void buyGame();

private:
    Ui::UserCartDialog *ui;

    int userId;

    /// SQL запрос для получения списка игр в корзине и всей информации для их отображения
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

    /// Список виджетов элементов игр в корзине
    QList<QWidget *> cartItems;

    /// Метод для обновления корзины
    void updateContent();
};

#endif // USERCARTDIALOG_H
