#ifndef USERCOLLECTIONDIALOG_H
#define USERCOLLECTIONDIALOG_H

#include <QDialog>

namespace Ui {
class UserCollectionDialog;
}

class UserCollectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserCollectionDialog(QWidget *parent = nullptr);
    ~UserCollectionDialog();

private slots:
    void on_sideList_currentRowChanged(int currentRow);

private:
    Ui::UserCollectionDialog *ui;
};

#endif // USERCOLLECTIONDIALOG_H
