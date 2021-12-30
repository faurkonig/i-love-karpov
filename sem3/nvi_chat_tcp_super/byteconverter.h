#ifndef BYTECONVERTER_H
#define BYTECONVERTER_H

#include <QHostAddress>
#include <QByteArray>


class ByteConverter
{
public:
    static QString addressToString(QHostAddress address);
    static QByteArray addressToBytes(QHostAddress address);
    static QHostAddress bytesToAddress(QByteArray ba);
    static QByteArray intToBytes(quint32 value);
    static quint32 bytesToInt(QByteArray ba);
};

#endif // BYTECONVERTER_H
