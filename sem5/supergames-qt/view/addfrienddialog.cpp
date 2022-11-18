#include "addfrienddialog.h"
#include "ui_addfrienddialog.h"
#include "utils/commonpatterns.h"
#include <QtSql/QSqlQuery>

AddFriendDialog::AddFriendDialog(QSqlDatabase *newDb, int currentUser, QWidget *parent) :
    QDialog(parent),
    DatabaseContainer(this, newDb),
    ui(new Ui::AddFriendDialog),
    sourceId(currentUser)
{
    ui->setupUi(this);
}

AddFriendDialog::~AddFriendDialog()
{
    delete ui;
}

void AddFriendDialog::on_addButton_clicked()
{
    // Проверяем введённый логин на корректность
    auto loginToAdd = ui->loginField->text().trimmed().toLower();
    if (!loginToAdd.contains(*CommonPatterns::loginRegex)) {
        showError("Неверный логин");
        return;
    }

    bool ok;
    // Проверяем существование такого пользователя в базе данных
    // Ищем ID и имя пользователя по логину
    auto foundUser = execQuery(QString("SELECT id, \"name\" FROM users WHERE login = '%1'")
                               .arg(loginToAdd), ok);
    if (!ok || foundUser.size() < 1) {
        showError("Пользователь с таким логином не найден");
        return;
    }
    foundUser.first();
    auto userId = foundUser.value(0).toInt();
    auto userName = foundUser.value(1).toString();

    // Сверяем ID найденного пользователя со своим
    if (userId == sourceId) {
        showError("Нельзя добавить в друзья самого себя");
        return;
    }

    // Проверяем, есть ли уже дружба с этим пользователем
    auto foundFriendship = execQuery(QString("SELECT id FROM internal.friendship "
                                             "WHERE \"source\" = %1 AND \"target\" = %2")
                                     .arg(sourceId).arg(userId), ok);
    if (!ok || foundFriendship.size() != 0) {
        showError("Этот пользователь уже у вас в друзьях");
        return;
    }

    // Если все предыдущие проверки прошли, то добавляем пользователя в друзья
    execQuery(QString("INSERT INTO internal.friendship (\"source\", \"target\") "
                      "VALUES (%1, %2)").arg(sourceId).arg(userId), ok);
    if (!ok) return;

    showSuccess("Пользователь \""+userName+"\" добавлен в друзья!");
}


void AddFriendDialog::on_exitButton_clicked()
{
    // Просто закрываем окно
    accept();
}

void AddFriendDialog::showError(QString text)
{
    // Просто выводим красный текст
    ui->infoLabel->setText("<font color=\"red\">"+text+"</font>");
}

void AddFriendDialog::showSuccess(QString text)
{
    // Просто выводим зелённый текст
    ui->infoLabel->setText("<font color=\"green\">"+text+"</font>");
}
