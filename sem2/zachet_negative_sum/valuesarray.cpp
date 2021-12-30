#include "valuesarray.h"

/// Конструктор
ValuesArray::ValuesArray()
{

}

/// Добавление значения
void ValuesArray::addValue(int value)
{
    values.append(value);
}

/// Удаление всех значений
void ValuesArray::clearAll()
{
    values.clear();
}

/// Нахождение и вывод наибольшего значения
int ValuesArray::negativeSum()
{
    int result = 0;
    for (int i = 0; i < values.length(); i++)
    {
        if (values[i] < 0)
            result += values[i];
    }
    return result;
}

/// Вывод количества значений
int ValuesArray::length()
{
    return values.length();
}
