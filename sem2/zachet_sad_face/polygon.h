#ifndef POLYGON_H
#define POLYGON_H

#include <QImage>
#include <QPainter>
#include <QPainterPath>

#include "paintpath.h"
#include "brush.h"


class Polygon : public PaintPath
{
public:
    Polygon();

    void draw(QImage &im, int n);

    Pen polyPen;
    Brush polyBrush;
};

#endif // POLYGON_H
