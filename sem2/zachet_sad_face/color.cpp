#include "color.h"

/// Конструктор
Color::Color()
{
    red = 0;
    green = 0;
    blue = 0;
    alpha = 255;
}

/// Установка цвета
bool Color::setRGBA(int r, int g, int b, int a)
{
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

/// Возвращение красного цвета
int Color::getRed()
{
    return red;
}

/// Возвращение зелёного цвета
int Color::getGreen()
{
    return green;
}

/// Возвращение синего цвета
int Color::getBlue()
{
    return blue;
}

/// Возвращение прозрачности
int Color::getAlpha()
{
    return alpha;
}

/// Возвращение всех цветов
void Color::getRGBA(int &r, int &g, int &b, int &a)
{
    r = red;
    g = green;
    b = blue;
    a = alpha;
}
