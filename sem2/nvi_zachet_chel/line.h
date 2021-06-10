#ifndef LINE_H
#define LINE_H

#include <QImage>
#include <QPainter>

#include "pen.h"
#include "point.h"


class line: public pen
{
public:
    line();

    point p1;
    point p2;

    void draw(QImage &im);
};

#endif // LINE_H
