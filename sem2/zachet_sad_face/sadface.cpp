#include "sadface.h"

/// Конструктор
SadFace::SadFace()
{

}

/// Рисование грустной рожицы
void SadFace::draw(QImage &im)
{
    int x = getX(), y = getY(), w = getRadius1(), h = getRadius2();

    Brush brush;
    brush.setRGBA(0, 0, 0, 0);

    // Основная часть - собственно лицо
    Ellipse mainEl;
    mainEl.setXY(x, y);
    mainEl.setRadius1(w);
    mainEl.setRadius2(h);
    mainEl.rectPen = rectPen;
    mainEl.rectBrush = brush;
    mainEl.draw(im);

    // Рот
    Arc mouth;
    mouth.setXY(x, y + (h*3)/4);
    mouth.setRadius1(w * 0.8);
    mouth.setRadius2(h * 0.7);
    mouth.setStartAngle(30);
    mouth.setSpanAngle(120);
    mouth.rectPen = rectPen;
    mouth.draw(im);

    int r, g, b, a;
    rectPen.getRGBA(r, g, b, a);
    brush.setRGBA(r, g, b, a);

    // Левый глаз
    Ellipse eye1;
    eye1.setXY(x - w/3, y - w/3);
    eye1.setRadius1(w/6);
    eye1.setRadius2(w/6);
    eye1.rectPen = rectPen;
    eye1.rectBrush = brush;
    eye1.draw(im);

    // Правый глаз
    Ellipse eye2;
    eye2.setXY(x + w/3, y - w/3);
    eye2.setRadius1(w/6);
    eye2.setRadius2(w/6);
    eye2.rectPen = rectPen;
    eye2.rectBrush = brush;
    eye2.draw(im);
}
