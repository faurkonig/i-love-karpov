#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include <QtSql/QSqlDatabase>

namespace Ui {
class ConnectionDialog;
}

struct ConnectionFields {
    QString address;
    int port;
    QString dbName;
    QString dbUser;
    QString dbPassword;
};

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(QSqlDatabase *newDb, bool *isSave, QWidget *parent = nullptr);
    ~ConnectionDialog();

private slots:
    void on_testConnectionButton_clicked();

    void on_pushButtonSave_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::ConnectionDialog *ui;
    QSqlDatabase *db;
    bool *save;

    ConnectionFields getFieldsData(bool &ok);

    void print(QString content);
    void printError(QString content);
    void printSuccess(QString content);
};

#endif // CONNECTIONDIALOG_H
