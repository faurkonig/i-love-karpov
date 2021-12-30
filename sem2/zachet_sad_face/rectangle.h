#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QImage>
#include <QPainter>

#include "point.h"
#include "pen.h"
#include "brush.h"


class Rectangle
{
public:
    Rectangle();

    Point p1;
    Point p2;
    Pen rectPen;
    Brush rectBrush;

    void draw(QImage &im);
};

#endif // RECTANGLE_H
