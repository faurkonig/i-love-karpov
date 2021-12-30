#include "charcounter.h"

// Конструктор
CharCounter::CharCounter()
{

}

// Установка значения строки. Возвращает false, если строка пустая
bool CharCounter::setString(QString value)
{
    if (value.isEmpty())
        return false;
    else
        str = value;

    return true;
}

// Установка значения символа. Возвращает false, если символ длины больше или меньше 1
bool CharCounter::setCharacter(QString value)
{
    if (value.isEmpty() || value.length() > 1)
        return false;
    else
        character = value;

    return true;
}

// Вывод результата - количества появления символа в строке
int CharCounter::getResult()
{
    return str.count(character);
}
