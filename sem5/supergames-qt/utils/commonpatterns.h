#ifndef COMMONPATTERNS_H
#define COMMONPATTERNS_H

#include <QRegExp>

/// Пространство имён, содержащее универсальные паттерны проверки и отображения
namespace CommonPatterns {
/// Регулярное выражение для проверки логина пользователя
static const QRegExp loginRegex("^[a-z][a-z0-9_]+$");

/// Очень простое и наивное регулярное выражение для проверки почты пользователя.
/// Есть, конечно, и умнее: https://www.emailregex.com
static const QRegExp emailRegex("^.+@.+\\..+$");

/// Типичный формат отображения даты ("день:месяц:год час:минуты")
static const QString dateTimeFormat("dd.MM.yyyy hh:mm");
}

#endif // COMMONPATTERNS_H
