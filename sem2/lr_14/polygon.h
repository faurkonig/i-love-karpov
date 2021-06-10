#ifndef POLYGON_H
#define POLYGON_H

#include <QImage>
#include <QPainter>
#include <QPainterPath>

#include "paintpath.h"
#include "brush.h"


class polygon : public paintpath
{
public:
    polygon();

    void draw(QImage &im, int n);

    pen polyPen;
    brush polyBrush;
};

#endif // POLYGON_H
