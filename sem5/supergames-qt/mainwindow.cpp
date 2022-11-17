#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectiondialog.h"
#include "registerdialog.h"
#include "storewindow.h"
#include "commonpatterns.h"
#include "dialoghelper.h"
#include "hash.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      DatabaseContainer(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectionButton_clicked()
{
    auto newConnection = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
    auto isSaved = new bool(false);
    auto connectionDialog = ConnectionDialog(newConnection, isSaved, this);
    // Открываю
    connectionDialog.exec();

    // Исполнится, когда закроется
    if (*isSaved) {
        if (mainDatabase != nullptr) {
            if (mainDatabase->isOpen()) mainDatabase->close();
            delete mainDatabase;
        }

        mainDatabase = newConnection;

        ui->signInBlock->setEnabled(true);
    }

    delete isSaved;
}


void MainWindow::on_radioButtonGuest_clicked()
{
    ui->formFrame->setEnabled(false);
}

void MainWindow::on_radioButtonUser_clicked()
{
    ui->formFrame->setEnabled(true);
    ui->loginLabel->setText("Логин:");
}

void MainWindow::on_radioButtonDeveloper_clicked()
{
    ui->formFrame->setEnabled(true);
    ui->loginLabel->setText("Почта:");
}


void MainWindow::on_loginPushButton_clicked()
{
    if (ui->radioButtonGuest->isChecked()) {
        // Вход как гость
        auto window = new StoreWindow(SgUser(0, "_guest", "", "Гость", QDateTime::currentDateTime()), mainDatabase);
        window->show();
    } else {
        // Вход как пользователь или разработчик

        // Определяем тип входа
        auto isUser = ui->radioButtonUser->isChecked();

        // Проверяем логин
        auto login = ui->loginField->text().trimmed();
        if (isUser) {
            // Для пользователя это просто логин
            if (!login.contains(CommonPatterns::loginRegex)) {
                DialogHelper::showValidationError(this, "Неверный логин");
                return;
            }
        } else {
            // Для разработчика это почта
            if (!login.contains(CommonPatterns::emailRegex)) {
                DialogHelper::showValidationError(this, "Неверная почта");
                return;
            }
        }

        // Проверяем пароль
        auto password = ui->passwordField->text().trimmed();
        if (password.isEmpty()) {
            DialogHelper::showValidationError(this, "Пароль не может быть пустым");
            return;
        }

        if (isUser) {
            // Входим как пользователь
            bool ok;
            auto user = loginAsUser(login, password, ok);

            if (!ok) {
                DialogHelper::showAuthError(this, isUser);
                return;
            }

            auto window = new StoreWindow(user, mainDatabase);
            window->show();
        }
    }
}

SgUser MainWindow::loginAsUser(QString login, QString password, bool &ok)
{
    SgUser user;

    auto hashedPassword = hashString(password);

    auto q = execQuery(QString("SELECT id, \"name\", \"date\" FROM users "
                                   "WHERE login = '%1' AND \"password\" = '%2'")
                       .arg(login, hashedPassword), ok);
    // Проверяем вообще, нашёлся ли такой пользователь
    if (!ok || q.size() < 1) {
        ok = false;
        return user;
    }
    q.first();

    // Записываем данные
    user.id = q.value(0).toInt();
    user.login = login;
    user.password = password;
    user.name = q.value(1).toString();
    user.date = q.value(2).toDateTime();

    ok = true;
    return user;
}

SgDeveloper MainWindow::loginAsDeveloper(QString login, QString password, bool &ok)
{
    SgDeveloper dev;

    auto hashedPassword = hashString(password);

    auto q = execQuery(QString("SELECT * FROM developers "
                               "WHERE email = '%1' AND \"password\" = '%2'")
                       .arg(login, hashedPassword), ok);
    // Проверяем вообще, нашёлся ли такой разработчик
    if (!ok || q.size() < 1) {
        ok = false;
        return dev;
    }
    q.first();

    ok = true;
    return dev;
}

void MainWindow::on_registerPushButton_clicked()
{
    auto registerDialog = RegisterDialog(mainDatabase, this);
    registerDialog.exec();
}


void MainWindow::on_loginField_returnPressed()
{
    focusNextChild();
}

void MainWindow::on_passwordField_returnPressed()
{
    on_loginPushButton_clicked();
}
