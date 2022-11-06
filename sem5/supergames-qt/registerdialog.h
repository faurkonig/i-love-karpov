#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include <QtSql/QSqlDatabase>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QSqlDatabase *newDb, QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_typeComboBox_currentIndexChanged(int index);

    void on_registerButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::RegisterDialog *ui;

    QSqlDatabase *db;

    bool isUser{true};

    bool registerUser(QString login, QString password, QString name);
    bool registerDeveloper(QString email, QString password, QString name);
};

#endif // REGISTERDIALOG_H
