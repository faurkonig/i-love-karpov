#include "connectiondialog.h"
#include "ui_connectiondialog.h"
#include <QtSql/QSqlError>
#include <QMessageBox>

// Нужно чтобы во время разработки автоматически подключатся с тестовыми данными
// В конечном итоге лучше закомментировать
#define SG_DEBUG

ConnectionDialog::ConnectionDialog(QSqlDatabase *newDb, bool *isSave, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog),
    db(newDb),
    save(isSave)
{
    ui->setupUi(this);

    on_testConnectionButton_clicked();

#ifdef SG_DEBUG
    on_pushButtonSave_clicked();
#endif
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}


void ConnectionDialog::on_testConnectionButton_clicked()
{
    bool correctFields;
    auto fields = getFieldsData(correctFields);
    if (correctFields) {
        print("Тест подключения...");
        db->setHostName(fields.address);
        db->setPort(fields.port);
        db->setDatabaseName(fields.dbName);
        db->setUserName(fields.dbUser);
        db->setPassword(fields.dbPassword);

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
        db->setHostName(fields.address);
        db->setPort(fields.port);
        db->setDatabaseName(fields.dbName);
        db->setUserName(fields.dbUser);
        db->setPassword(fields.dbPassword);

        if (!db->open()) {
            printError("Не удалось подключится к базе данных. Проверьте данные для подключения");
            printError(db->lastError().text());
            return;
        }

        *save = true;

#ifndef SG_DEBUG
        QMessageBox::information(this, "Подключение", "Успешное подключение к базе данных");
#endif
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

    fields.address = ui->hostField->text();
    bool correctPort;
    fields.port = ui->portField->text().toInt(&correctPort);
    fields.dbName = ui->dbNameField->text();
    fields.dbUser = ui->dbUserField->text();
    fields.dbPassword = ui->dbPasswordField->text();

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
