#ifndef LINE_H
#define LINE_H

#include <primitive.h>
#include <point.h>


class Line : public Primitive
{
public:
    Line();

    Point p1;
    Point p2;

    void draw(QImage * im) override;
};

#endif // LINE_H
