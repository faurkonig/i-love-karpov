#include "color.h"

Color::Color()
{
    // Конструктор
    red = 0;
    green = 0;
    blue = 0;
    alpha = 255;
}

bool Color::setRGBA(int r, int g, int b, int a)
{
    // Установка цвета
    if (r < 0 || r > 255 ||
            g < 0 || g > 255 ||
            b < 0 || b > 255 ||
            a < 0 || a > 255) return false;
    red = r;
    green = g;
    blue = b;
    alpha = a;
    return true;
}

int Color::getRed()
{
    // Возвращение красного цвета
    return red;
}

int Color::getGreen()
{
    // Возвращение зелёного цвета
    return green;
}

int Color::getBlue()
{
    // Возвращение синего цвета
    return blue;
}

int Color::getAlpha()
{
    // Возвращение прозрачности
    return alpha;
}

void Color::getRGBA(int &r, int &g, int &b, int &a)
{
    // Возвращение всех цветов
    r = red;
    g = green;
    b = blue;
    a = alpha;
}
