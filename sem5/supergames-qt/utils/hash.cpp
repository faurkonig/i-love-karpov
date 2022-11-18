#include "hash.h"
#include <QCryptographicHash>

QString hashString(QString raw) {
    // Создания объекта хэша и запись данных в него
    QCryptographicHash crypto(QCryptographicHash::Md5);
    crypto.addData(raw.toUtf8());
    // Собственно хэширование и превращение результата в строку
    return QString::fromUtf8(crypto.result().toHex());
}
