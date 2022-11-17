#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

#include "databasecontainer.h"
#include <QDialog>

namespace Ui {
class AddFriendDialog;
}

class AddFriendDialog : public QDialog, private DatabaseContainer
{
    Q_OBJECT

public:
    /// Конструктор
    explicit AddFriendDialog(QSqlDatabase *newDb, int currentUser, QWidget *parent = nullptr);
    /// Деструктор
    ~AddFriendDialog();

private slots:
    /// Обработчик нажатия на кнопку "Добавить друга"
    void on_addButton_clicked();

    /// Обработчик нажатия на кнопку "Отмена"
    void on_exitButton_clicked();

private:
    Ui::AddFriendDialog *ui;

    int sourceId;

    /// Вывод ошибки в строку с информацией
    void showError(QString text);
    /// Вывод информации об успешном добавлении в друзья
    void showSuccess(QString text);
};

#endif // ADDFRIENDDIALOG_H
