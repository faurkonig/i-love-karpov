#include "byteconverter.h"


/// Метод для превращения адреса в красивую строку
QString ByteConverter::addressToString(QHostAddress address)
{
    return address.toString().split(':').last();
}

/// Метод для превращения адреса в массив байтов
QByteArray ByteConverter::addressToBytes(QHostAddress address)
{
    return intToBytes(address.toIPv4Address());
}

/// Метод для превращения массива байтов в адрес
QHostAddress ByteConverter::bytesToAddress(QByteArray ba)
{
    quint32 addrInt = bytesToInt(ba);
    return QHostAddress(addrInt);
}

/// Метод для превращения числа в массив байтов
QByteArray ByteConverter::intToBytes(quint32 value)
{
    char *intAsBytes = new char[4];
    memcpy(intAsBytes, &value, 4);
    QByteArray ba(intAsBytes, 4);
    delete [] intAsBytes;
    return ba;
}

/// Метод для превращения массива байтов в число
quint32 ByteConverter::bytesToInt(QByteArray ba)
{
    quint32 result;
    memcpy(&result, ba.constData(), 4);
    return result;
}
