#include "elliptic.h"

Elliptic::Elliptic()
{
    // Конструктор
    radius1 = 0;
    radius2 = 0;
}

bool Elliptic::setRadius1(int val)
{
    // Установка первого радиуса (по ширине)
    if (val < 0 || val > INTMAX) return false;
    radius1 = val;
    return true;
}

bool Elliptic::setRadius2(int val)
{
    // Установка второго радиуса (по высоте)
    if (val < 0 || val > INTMAX) return false;
    radius2 = val;
    return true;
}

int Elliptic::getRadius1()
{
    // Возвращение первого радиуса
    return radius1;
}

int Elliptic::getRadius2()
{
    // Возвращение второго радиуса
    return radius2;
}
