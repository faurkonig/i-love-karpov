#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include <QtSql/QSqlError>
#include <QMessageBox>


ConnectionDialog::ConnectionDialog(QSqlDatabase *newDb, bool *isSave, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog),
    db(newDb),
    save(isSave)
{
    ui->setupUi(this);

    // Автоматически проверяем соединение со стандартными параметрами
    on_testConnectionButton_clicked();
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;

    if (!isSavedInside) delete db;
}


void ConnectionDialog::on_testConnectionButton_clicked()
{
    bool correctFields;
    auto fields = getFieldsData(correctFields);
    if (correctFields) {
        print("Тест подключения...");
        // Записываем значения
        db->setHostName(fields.address);
        db->setPort(fields.port);
        db->setDatabaseName(fields.dbName);
        db->setUserName(fields.dbUser);
        db->setPassword(fields.dbPassword);

        // Проверяем соединение
        if (db->open()) {
            printSuccess("Соединение с базой данных успешно установлено! Данные корректные");
            db->close();
        } else {
            printError("Не удалось подключится к базе данных. Проверьте данные для подключения");
            printError(db->lastError().text());
        }
    } else {
        printError("Введены неверные данные");
    }
}

void ConnectionDialog::on_pushButtonSave_clicked()
{
    bool correctFields;
    auto fields = getFieldsData(correctFields);
    if (correctFields) {
        // Записываем значения
        db->setHostName(fields.address);
        db->setPort(fields.port);
        db->setDatabaseName(fields.dbName);
        db->setUserName(fields.dbUser);
        db->setPassword(fields.dbPassword);

        // Проверяем соединение
        if (!db->open()) {
            printError("Не удалось подключится к базе данных. Проверьте данные для подключения");
            printError(db->lastError().text());
            return;
        }

        *save = true;
        // Устанавливаем этот флаг чтобы БД не удалилась после закрытия диалога
        isSavedInside = true;

        QMessageBox::information(this, "Подключение", "Успешное подключение к базе данных");
        accept();
    } else {
        printError("Введены неверные данные, невозможно сохранить");
    }
}

void ConnectionDialog::on_pushButtonCancel_clicked()
{
    reject();
}


ConnectionFields ConnectionDialog::getFieldsData(bool &ok)
{
    ConnectionFields fields;
    ok = true;

    // Сохраняем поля по введённым данным
    fields.address = ui->hostField->text();
    bool correctPort;
    fields.port = ui->portField->text().toInt(&correctPort);
    fields.dbName = ui->dbNameField->text();
    fields.dbUser = ui->dbUserField->text();
    fields.dbPassword = ui->dbPasswordField->text();

    // Проверяем поля
    if (fields.address.isEmpty()) {
        printError("Адрес хоста не может быть пустым");
        ok = false;
    }
    if (!correctPort || fields.port <= 0) {
        printError("Некорректный порт");
        ok = false;
    }
    if (fields.dbName.isEmpty()) {
        printError("Имя базы данных не может быть пустым");
        ok = false;
    }
    if (fields.dbUser.isEmpty()) {
        printError("Имя пользователя базы данных не может быть пустым");
        ok = false;
    }
    if (fields.dbPassword.isEmpty()) {
        printError("Пароль от базы данных не может быть пустым");
        ok = false;
    }

    return fields;
}


void ConnectionDialog::print(QString content)
{
    ui->connectionOutput->append(content);
}

void ConnectionDialog::printError(QString content)
{
    ui->connectionOutput->append("<span style=\"color:red;\">" + content + "</span>");
}

void ConnectionDialog::printSuccess(QString content)
{
    ui->connectionOutput->append("<span style=\"color:green;\">" + content + "</span>");
}
