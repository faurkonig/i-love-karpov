#include "point.h"

#include "consts.h"

Point::Point()
{
    // Конструктор
    x = 0;
    y = 0;
}

bool Point::setX(int new_x)
{
    // Установка координаты X
    if (new_x < 0 || new_x > INTMAX) return false;
    x = new_x;
    return true;
}

bool Point::setY(int new_y)
{
    // Установка координаты Y
    if (new_y < 0 || new_y > INTMAX) return false;
    y = new_y;
    return true;
}

bool Point::setXY(int new_x, int new_y)
{
    // Установка обоих координат
    if (new_x < 0 || new_x > INTMAX ||
            new_y < 0 || new_y > INTMAX) return false;
    x = new_x;
    y = new_y;
    return true;
}

int Point::getX()
{
    // Возвращение координаты X
    return x;
}

int Point::getY()
{
    // Возвращение координаты Y
    return y;
}

void Point::getXY(int &new_x, int &new_y)
{
    // Возвращение обоих координат
    new_x = x;
    new_y = y;
}
