#ifndef CURVE_H
#define CURVE_H

#include <QImage>
#include <QPainter>
#include <QPainterPath>

#include "paintpath.h"


class Curve : public PaintPath, public Pen
{
public:
    Curve();

    void draw(QImage &im, int n);
};

#endif // CURVE_H
