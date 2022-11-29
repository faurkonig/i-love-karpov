#ifndef USERCARTDIALOG_H
#define USERCARTDIALOG_H

#include "utils/databasecontainer.h"
#include <QDialog>

namespace Ui {
/// Диалог с корзиной пользователя
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

    QList<QWidget *> cartItems;

    /// Метод для обновления корзины
    void updateContent();
};

#endif // USERCARTDIALOG_H
