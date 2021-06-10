#include "pen.h"

#include "consts.h"

Pen::Pen()
{
    // Конструктор
    weight = 1;
    style = 1;
}

bool Pen::setWeight(int w)
{
    // Установка ширины линии
    if (w < 0 || w > 1000) return false;
    weight = w;
    return true;
}

bool Pen::setStyle(int s)
{
    // Установка стиля заливки
    if (s < 0 || s > 5) return false;
    style = s;
    return true;
}

int Pen::getWeight()
{
    // Возвращение ширины заливки
    return weight;
}

int Pen::getStyle()
{
    // Возвращение стиля заливки
    return style;
}
