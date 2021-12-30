#include "pen.h"

/// Конструктор
Pen::Pen()
{
    width = 1;
    style = 1;
}

/// Установка ширины линии
bool Pen::setWidth(int w)
{
    if (w < 0) return false;
    width = w;
    return true;
}

/// Установка стиля заливки
bool Pen::setStyle(int s)
{
    if (s < 0 || s > 5) return false;
    style = s;
    return true;
}

/// Возвращение ширины заливки
int Pen::getWidth()
{
    return width;
}

/// Возвращение стиля заливки
int Pen::getStyle()
{
    return style;
}
