#include "brush.h"

Brush::Brush()
{
    // Конструктор
    style = 1;
}

bool Brush::setStyle(int s)
{
    // Установка стиля заливки
    if (s < 0 || s > 14) return false;
    style = s;
    return true;
}

int Brush::getStyle()
{
    // Возвращение стиля заливки
    return style;
}
