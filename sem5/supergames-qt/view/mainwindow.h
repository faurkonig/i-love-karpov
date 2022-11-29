#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "sguser.h"
#include "sgdeveloper.h"
#include "utils/databasecontainer.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/// Главное окно (вход в аккаунты)
class MainWindow : public QMainWindow, private DatabaseContainer
{
    Q_OBJECT

public:
    /// Конструктор
    MainWindow(QWidget *parent = nullptr);
    /// Деструктор
    ~MainWindow();

private slots:
    /// Обработчик нажатия на кнопку подключения к БД
    void on_connectionButton_clicked();

    /// Обработчик выбор входа как пользователь
    void on_radioButtonUser_clicked();

    /// Обработчик выбор входа как гость
    void on_radioButtonGuest_clicked();

    /// Обработчик выбор входа как разработчик
    void on_radioButtonDeveloper_clicked();

    /// Обработчик нажатия на кнопку регистрации аккаунта
    void on_registerPushButton_clicked();

    /// Обработчик нажатия на кнопку входа в аккаунт
    void on_loginPushButton_clicked();

    /// Обработчик нажатия клавиши Enter при выбранном поле логина
    void on_loginField_returnPressed();

    /// Обработчик нажатия клавиши Enter при выбранном поле пароля
    void on_passwordField_returnPressed();

private:
    Ui::MainWindow *ui;

    /// Метод для входа в аккаунт пользователя
    SgUser loginAsUser(QString login, QString password, bool &ok);
    /// Метод для входа в аккаунт разработчика
    SgDeveloper loginAsDeveloper(QString login, QString password, bool &ok);
};
#endif // MAINWINDOW_H
