#include "point.h"

/// Конструктор
Point::Point()
{
    x = 0;
    y = 0;
}

/// Установка координаты X
bool Point::setX(int new_x)
{
    if (new_x < 0) return false;
    x = new_x;
    return true;
}

/// Установка координаты Y
bool Point::setY(int new_y)
{
    if (new_y < 0) return false;
    y = new_y;
    return true;
}

/// Установка обоих координат
bool Point::setXY(int new_x, int new_y)
{
    if (new_x < 0 || new_y < 0) return false;
    x = new_x;
    y = new_y;
    return true;
}

/// Возвращение координаты X
int Point::getX()
{
    return x;
}

/// Возвращение координаты Y
int Point::getY()
{
    return y;
}

/// Возвращение обоих координат
void Point::getXY(int &new_x, int &new_y)
{
    new_x = x;
    new_y = y;
}
