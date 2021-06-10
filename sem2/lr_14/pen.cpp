#include "pen.h"

#include "consts.h"

pen::pen()
{
    // Конструктор
    width = 1;
    style = 1;
}

bool pen::setWidth(int w)
{
    // Установка ширины линии
    if (w < 0 || w > 1000)
    {
        return false;
    }
    else
    {
        width = w;
        return true;
    }
}

bool pen::setStyle(int s)
{
    // Установка стиля заливки
    if (s < 0 || s > 5)
    {
        return false;
    }
    else
    {
        style = s;
        return true;
    }
}

int pen::getWidth()
{
    // Возвращение ширины заливки
    return width;
}

int pen::getStyle()
{
    // Возвращение стиля заливки
    return style;
}
