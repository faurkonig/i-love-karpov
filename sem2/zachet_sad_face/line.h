#ifndef LINE_H
#define LINE_H

#include <QImage>
#include <QPainter>

#include "pen.h"
#include "point.h"


class Line: public Pen
{
public:
    Line();

    Point p1;
    Point p2;

    void draw(QImage &im);
};

#endif // LINE_H
