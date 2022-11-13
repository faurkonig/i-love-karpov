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
    explicit AddFriendDialog(QSqlDatabase *newDb, int currentUser, QWidget *parent = nullptr);
    ~AddFriendDialog();

private slots:
    void on_addButton_clicked();

    void on_exitButton_clicked();

private:
    Ui::AddFriendDialog *ui;

    int sourceId;

    void showError(QString text);
    void showSuccess(QString text);
};

#endif // ADDFRIENDDIALOG_H
