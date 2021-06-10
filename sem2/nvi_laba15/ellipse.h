#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <primitive.h>
#include <elliptic.h>
#include <brush.h>


class Ellipse: public Primitive, public Elliptic
{
public:
    Ellipse();

    void draw(QImage * im) override;
};

#endif // ELLIPSE_H
