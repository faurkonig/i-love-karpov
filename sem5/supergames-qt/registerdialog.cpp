#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "dialoghelper.h"
#include "hash.h"
#include "commonpatterns.h"
#include <QMessageBox>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QCryptographicHash>

RegisterDialog::RegisterDialog(QSqlDatabase *newDb, QWidget *parent) :
    QDialog(parent),
    DatabaseContainer(parent, newDb),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}


void RegisterDialog::on_typeComboBox_currentIndexChanged(int index)
{
    if (index == 0) {
        isUser = true;
        ui->userLoginTitle->setText("Логин:");
    } else {
        isUser = false;
        ui->userLoginTitle->setText("Почта:");
    }
}

void RegisterDialog::on_registerButton_clicked()
{
    auto login = ui->userLoginField->text().trimmed().toLower();
    if (isUser){
        if (login.length() < 4) {
            DialogHelper::showValidationError(this, "Логин должен быть длиной от 4 символов");
            return;
        }
        if (!login.contains(CommonPatterns::loginRegex)) {
            DialogHelper::showValidationError(this, "Логин должен состоять из латинских букв, цифр или знака \"_\""
                                                    " (и может начинатся только с латинской буквы)");
            return;
        }
    } else if (!login.contains(CommonPatterns::emailRegex)) {
        DialogHelper::showValidationError(this, "Введена некорректная почта");
        return;
    }

    auto password = ui->userPasswordField->text().trimmed();
    if (password.length() < 4) {
        DialogHelper::showValidationError(this, "Пароль должен быть длиной от 5 символов");
        return;
    }

    auto repeatedPassword = ui->userPasswordRepeatField->text().trimmed();
    if (repeatedPassword != password) {
        DialogHelper::showValidationError(this, "Пароли не совпадают");
        return;
    }

    auto name = ui->userNameField->text().trimmed();
    if (name.isEmpty()) {
        DialogHelper::showValidationError(this, "Имя не может быть пустым");
        return;
    }

    if (isUser) {
        // Пробуем зарегистрировать пользователя
        if (registerUser(login, password, name)) {
            QMessageBox::information(this, "Регистрация", "Пользователь успешно зарегистрирован");
            accept();
        }
    } else {
        // Пробуем зарегистрировать разработчика
        if (registerDeveloper(login, password, name)) {
            QMessageBox::information(this, "Регистрация", "Разработчик успешно зарегистрирован");
            accept();
        }
    }
}

void RegisterDialog::on_cancelButton_clicked()
{
    reject();
}


bool RegisterDialog::registerUser(QString login, QString password, QString name)
{
    if (!checkDatabase()) return false;

    // Хэшируем пароль, чтобы в БД он не лежал в открытом виде
    auto hashedPassword = hashString(password);

    auto query = QString("INSERT INTO \"users\" (login, password, name) VALUES ('%1', '%2', '%3');")
            .arg(login, hashedPassword, name);
    auto q = mainDatabase->exec(query);
    if (DialogHelper::isSqlError(q.lastError())) {
        DialogHelper::showSqlError(this, q.lastError(), query);
        return false;
    }
    return true;
}

bool RegisterDialog::registerDeveloper(QString email, QString password, QString name)
{
    if (!checkDatabase()) return false;

    // Хэшируем пароль, чтобы в БД он не лежал в открытом виде
    auto hashedPassword = hashString(password);

    auto query = QString("INSERT INTO \"developers\" (\"email\", \"password\", \"name\") VALUES ('%1', '%2', '%3');")
            .arg(email, hashedPassword, name);
    auto q = mainDatabase->exec(query);
    if (DialogHelper::isSqlError(q.lastError())) {
        DialogHelper::showSqlError(this, q.lastError(), query);
        return false;
    }
    return true;
}
