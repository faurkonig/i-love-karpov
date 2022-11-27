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

    QList<int> gameIds;

    /// Метод для обновления списка в боковом меню
    void updateSideList();
};

#endif // USERCOLLECTIONDIALOG_H
