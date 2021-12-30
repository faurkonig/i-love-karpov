#include "maxvaluearray.h"

/// Конструктор
MaxValueArray::MaxValueArray()
{

}

/// Добавление значения
void MaxValueArray::addValue(int value)
{
    values.append(value);
}

/// Удаление всех значений
void MaxValueArray::clearAll()
{
    values.clear();
}

/// Нахождение и вывод наибольшего значения
int MaxValueArray::maxValue()
{
    int result = values[0];
    for (int i = 1; i < values.length(); i++)
    {
        if (values[i] > result)
            result = values[i];
    }
    return result;
}

/// Вывод количества значений
int MaxValueArray::length()
{
    return values.length();
}
