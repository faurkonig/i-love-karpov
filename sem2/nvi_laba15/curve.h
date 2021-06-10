#ifndef CURVE_H
#define CURVE_H

#include <QPainterPath>

#include <primitive.h>
#include <paintpath.h>


class Curve : public Primitive
{
public:
    Curve();

    void draw(QImage * im) override;
};

#endif // CURVE_H
