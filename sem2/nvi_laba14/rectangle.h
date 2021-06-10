#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QImage>
#include <QPainter>

#include "point.h"
#include "pen.h"
#include "brush.h"


class rectangle
{
public:
    rectangle();

    point p1;
    point p2;
    pen rectPen;
    brush rectBrush;

    void draw(QImage &im);
};

#endif // RECTANGLE_H
