#ifndef PIXEL_H
#define PIXEL_H

#include <primitive.h>
#include <point.h>


class Pixel : public Primitive, public Point
{
public:
    Pixel();

    void draw(QImage * im) override;
};

#endif // PIXEL_H
