#include "elliptic.h"

/// Конструктор
Elliptic::Elliptic()
{
    radius1 = 0;
    radius2 = 0;
}

/// Установка первого радиуса (по ширине)
bool Elliptic::setRadius1(int val)
{
    if (val < 0) return false;
    radius1 = val;
    return true;
}

/// Установка второго радиуса (по высоте)
bool Elliptic::setRadius2(int val)
{
    if (val < 0) return false;
    radius2 = val;
    return true;
}

/// Возвращение первого радиуса
int Elliptic::getRadius1()
{
    return radius1;
}

/// Возвращение второго радиуса
int Elliptic::getRadius2()
{
    return radius2;
}
