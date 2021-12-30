#ifndef STRINGCOMPARE_H
#define STRINGCOMPARE_H

#include <QString>

class StringCompare
{
public:
    StringCompare();

    void setStr1(QString value);
    void setStr2(QString value);

    bool compare();

private:
    QString str1;
    QString str2;
};

#endif // STRINGCOMPARE_H
