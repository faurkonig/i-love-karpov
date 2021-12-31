#ifndef PIXEL_H
#define PIXEL_H

#include <QImage>
#include <QPainter>

#include "point.h"
#include "pen.h"


class Pixel : public Pen, public Point
{
public:
    Pixel();

    void draw(QImage &im);
};

#endif // PIXEL_H
