#include "elliptic.h"

#include "consts.h"

elliptic::elliptic()
{
    // Конструктор
    radius1 = 0;
    radius2 = 0;
}

bool elliptic::setRadius1(int val)
{
    // Установка первого радиуса (по ширине)
    if (val < 0 || val > INTMAX) return false;
    radius1 = val;
    return true;
}

bool elliptic::setRadius2(int val)
{
    // Установка второго радиуса (по высоте)
    if (val < 0 || val > INTMAX) return false;
    radius2 = val;
    return true;
}

int elliptic::getRadius1()
{
    // Возвращение первого радиуса
    return radius1;
}

int elliptic::getRadius2()
{
    // Возвращение второго радиуса
    return radius2;
}
