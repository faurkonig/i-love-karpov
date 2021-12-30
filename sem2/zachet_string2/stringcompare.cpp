#include "stringcompare.h"

/// Конструктор
StringCompare::StringCompare()
{

}

/// Установка строки 1
void StringCompare::setStr1(QString value)
{
    str1 = value;
}

/// Установка строки 2
void StringCompare::setStr2(QString value)
{
    str2 = value;
}

/// Сравнение установленных строк
bool StringCompare::compare()
{
    return str1 == str2;
}
