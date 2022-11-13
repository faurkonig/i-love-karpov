#ifndef COMMONPATTERNS_H
#define COMMONPATTERNS_H

#include <QRegExp>

namespace CommonPatterns {
/// Регулярное выражение для проверки логина пользователя
const QRegExp loginRegex{"^[a-z][a-z0-9_]+$"};

/// Очень простое и наивное регулярное выражение для проверки почты пользователя
/// Но есть, конечно, и умнее: https://www.emailregex.com
/// Просто с ним можно замучатся
const QRegExp emailRegex{"^.+@.+\\..+$"};

const QString dateTimeFormat{"dd.MM.yyyy hh:mm"};
}

#endif // COMMONPATTERNS_H
