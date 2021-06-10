#include "point.h"

#include "consts.h"

Point::Point()
{
    // Конструктор
    x = 0;
    y = 0;
}

bool Point::setX(int newX)
{
    // Установка координаты X
    if (newX < 0 || newX > INTMAX) return false;
    x = newX;
    return true;
}

bool Point::setY(int newY)
{
    // Установка координаты Y
    if (newY < 0 || newY > INTMAX) return false;
    y = newY;
    return true;
}

bool Point::setXY(int newX, int newY)
{
    // Установка обоих координат
    if (newX < 0 || newX > INTMAX ||
            newY < 0 || newY > INTMAX) return false;
    x = newX;
    y = newY;
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

void Point::getXY(int &newX, int &newY)
{
    // Возвращение обоих координат
    newX = x;
    newY = y;
}
