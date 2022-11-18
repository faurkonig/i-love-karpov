#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include "utils/databasecontainer.h"
#include <QDialog>

namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog, private DatabaseContainer
{
    Q_OBJECT

public:
    /// Конструктор.
    /// Требует БД для выполенения запросов
    explicit RegisterDialog(QSqlDatabase *newDb, QWidget *parent = nullptr);
    /// Конструктор
    ~RegisterDialog();

private slots:
    /// Обработчик выбора типа аккаунта, который надо зарегистрировать
    void on_typeComboBox_currentIndexChanged(int index);

    /// Обработчик нажатия на кнопку регистрации
    void on_registerButton_clicked();

    /// Обработчик нажатия на кнопку отмены
    void on_cancelButton_clicked();

private:
    Ui::RegisterDialog *ui;

    /// Флаг для проверки режима регистрации.
    /// Значение меняется в слоте [on_typeComboBox_currentIndexChanged]
    bool isUser{true};

    /// Метод для регистрации аккаунта пользователя
    bool registerUser(QString login, QString password, QString name);
    /// Метод для регистрации аккаунта разработчика
    bool registerDeveloper(QString email, QString password, QString name);
};

#endif // REGISTERDIALOG_H
