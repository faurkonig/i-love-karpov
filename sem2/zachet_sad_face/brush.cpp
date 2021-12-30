#include "brush.h"

/// Конструктор
Brush::Brush()
{
    style = 1;
}

/// Установка стиля заливки
bool Brush::setStyle(int s)
{
    if (s < 0 || s > 14) return false;
    style = s;
    return true;
}

/// Возвращение стиля заливки
int Brush::getStyle()
{
    return style;
}
