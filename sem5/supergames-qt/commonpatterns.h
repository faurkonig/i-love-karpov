#ifndef COMMONPATTERNS_H
#define COMMONPATTERNS_H

#include <QRegExp>

namespace CommonPatterns {
/// Регулярное выражение для проверки логина пользователя
Q_GLOBAL_STATIC_WITH_ARGS(QRegExp, loginRegex, ("^[a-z][a-z0-9_]+$"));

/// Очень простое и наивное регулярное выражение для проверки почты пользователя.
/// Есть, конечно, и умнее: https://www.emailregex.com
Q_GLOBAL_STATIC_WITH_ARGS(QRegExp, emailRegex, ("^.+@.+\\..+$"));

/// Типичный формат отображения даты ("день:месяц:год час:минуты")
Q_GLOBAL_STATIC_WITH_ARGS(QString, dateTimeFormat, ("dd.MM.yyyy hh:mm"));
}

#endif // COMMONPATTERNS_H
