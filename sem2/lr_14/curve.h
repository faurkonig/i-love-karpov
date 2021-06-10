#ifndef CURVE_H
#define CURVE_H

#include <QImage>
#include <QPainter>
#include <QPainterPath>

#include "paintpath.h"


class curve : public paintpath, public pen
{
public:
    curve();

    void draw(QImage &im, int n);
};

#endif // CURVE_H
