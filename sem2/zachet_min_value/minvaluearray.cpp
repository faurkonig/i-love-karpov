#include "minvaluearray.h"

/// Конструктор
MinValueArray::MinValueArray()
{

}

/// Добавление значения
void MinValueArray::addValue(int value)
{
    values.append(value);
}

/// Удаление всех значений
void MinValueArray::clearAll()
{
    values.clear();
}

/// Нахождение и вывод наименьшего значения
int MinValueArray::minValue()
{
    int result = values[0];
    for (int i = 1; i < values.length(); i++)
    {
        if (values[i] < result)
            result = values[i];
    }
    return result;
}

/// Вывод количества значений
int MinValueArray::length()
{
    return values.length();
}
