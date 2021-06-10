#include "brush.h"

brush::brush()
{
    // Конструктор
    style = 1;
}

bool brush::setStyle(int s)
{
    // Установка стиля заливки
    if (s < 0 || s > 14) return false;
    style = s;
    return true;
}

int brush::getStyle()
{
    // Возвращение стиля заливки
    return style;
}
