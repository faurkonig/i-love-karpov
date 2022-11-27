#ifndef DIALOGHELPER_H
#define DIALOGHELPER_H

#include <QtSql/QSqlError>
#include <QWidget>
#include <QDateTime>

namespace DialogHelper {
/// Проверка запроса на ошибки
bool isSqlError(QSqlError error);

/// Вывод ошибки о неправильном вводе
void showValidationError(QWidget *parent, QString text);

/// Вывод ошибки базы данных
void showDatabaseError(QWidget *parent, QString text);

/// Вывод ошибки после выполненного SQL запроса
void showSqlError(QWidget *parent, QSqlError error, QString query);

/// Вывод ошибки авторизации
void showAuthError(QWidget *parent, bool isUser);

/// Форматирование времени по UTC в текущее
QString formatTime(QDateTime time);
}

#endif // DIALOGHELPER_H
