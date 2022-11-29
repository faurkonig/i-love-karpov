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

/// Диалог подключения к базе данных
class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    /// Конструктор
    explicit ConnectionDialog(QSqlDatabase *newDb, bool *isSave, QWidget *parent = nullptr);
    /// Деструктор
    ~ConnectionDialog();

private slots:
    /// Обработка нажатия на тест подключения
    void on_testConnectionButton_clicked();

    /// Обработка нажатия на кнопку сохранения настроек подключения базы данных
    void on_pushButtonSave_clicked();

    /// Обработка нажатия на кнопку отмены
    void on_pushButtonCancel_clicked();

private:
    Ui::ConnectionDialog *ui;
    QSqlDatabase *db;
    bool *save;

    bool isSavedInside{false};

    ConnectionFields getFieldsData(bool &ok);

    /// Вывод информации в "консоль"
    void print(QString content);
    /// Вывод ошибки в "консоль".
    /// Красный текст
    void printError(QString content);
    /// Вывод информации об успехе в "консоль".
    /// Залёный текст
    void printSuccess(QString content);
};

#endif // CONNECTIONDIALOG_H
