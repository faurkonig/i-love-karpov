#include "dialoghelper.h"
#include <QMessageBox>

bool DialogHelper::isSqlError(QSqlError error) {
    return error.type() != QSqlError::NoError;
}

void DialogHelper::showValidationError(QWidget *parent, QString text) {
    QMessageBox::critical(parent, "Ошибка валидации", "Введены неверные данные: " + text);
}

void DialogHelper::showDatabaseError(QWidget *parent, QString text) {
    QMessageBox::critical(parent, "Ошибка базы данных", text);
}

void DialogHelper::showSqlError(QWidget *parent, QSqlError error, QString query) {
    showDatabaseError(parent,
                      QString("Не удалось выполнить SQL запрос:\n%1\n\nОшибка:\n%2")
                      .arg(query, error.text()));
}

void DialogHelper::showAuthError(QWidget *parent, bool isUser)
{
    QString errorBase = "\nСкорее всего логин или пароль введены неверно";
    QMessageBox::warning(parent,
                         "Ошибка авторазации",
                         (isUser ? "Пользователь не найден" : "Разработчик не найден") + errorBase);
}
