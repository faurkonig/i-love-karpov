#ifndef POLYGON_H
#define POLYGON_H

#include <QPainterPath>

#include <primitive.h>
#include <paintpath.h>


class Polygon : public Primitive
{
public:
    Polygon();

    void draw(QImage * im) override;
};

#endif // POLYGON_H
