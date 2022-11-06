#ifndef COMMONPATTERNS_H
#define COMMONPATTERNS_H

#include <QRegExp>

namespace CommonPatterns {
/// Регулярное выражение для проверки логина пользователя
static auto loginRegex = QRegExp("^[a-z][a-z0-9_]+$");

/// Очень простое и наивное регулярное выражение для проверки почты пользователя
/// Но есть, конечно, и умнее: https://www.emailregex.com
/// Просто с ним можно замучатся
static auto emailRegex = QRegExp("^.+@.+\\..+$");
}

#endif // COMMONPATTERNS_H
