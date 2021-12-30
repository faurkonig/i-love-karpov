#ifndef CHARCOUNTER_H
#define CHARCOUNTER_H

#include <QString>


class CharCounter
{
public:
    CharCounter();

    bool setString(QString value);
    bool setCharacter(QString value);

    int getResult();

private:
    QString str;
    QString character;
};

#endif // CHARCOUNTER_H
