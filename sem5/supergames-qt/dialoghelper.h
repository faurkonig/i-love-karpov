#ifndef DIALOGHELPER_H
#define DIALOGHELPER_H

#include <QtSql/QSqlError>
#include <QWidget>

namespace DialogHelper {
/// Проверка запроса
bool isSqlError(QSqlError error);

void showValidationError(QWidget *parent, QString text);

void showDatabaseError(QWidget *parent, QString text);

void showSqlError(QWidget *parent, QSqlError error, QString query);

void showAuthError(QWidget *parent, bool isUser);
}

#endif // DIALOGHELPER_H
