#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <QImage>
#include <QPainter>

#include <primitive.h>
#include <point.h>


class Rectangle : public Primitive
{
public:
    Rectangle();

    Point p1;
    Point p2;

    void draw(QImage * im) override;
};

#endif // RECTANGLE_H
