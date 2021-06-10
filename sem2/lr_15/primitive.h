#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include <QImage>
#include <QPainter>
#include <QVector>

#include <pen.h>
#include <brush.h>
#include <point.h>


enum PrimitiveTypes {
    PixelT,
    LineT,
    RectangleT,
    EllipseT,
    ArcT,
    CurveT,
    PolygonT
};


class Primitive
{
public:
    Primitive();

    PrimitiveTypes type;

    Pen * pen = nullptr;
    Brush * brush = nullptr;
    QVector<Point> * points = nullptr;

    virtual void draw(QImage * im) = 0;
};

#endif // PRIMITIVE_H
