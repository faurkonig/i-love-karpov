#ifndef HASH_H
#define HASH_H

#include <QString>

/// Хэширование строки при помощи MD5.
/// Результат возвращается в виде строки в хексе (16-сс).
QString hashString(QString raw);

#endif // HASH_H
