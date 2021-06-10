#ifndef ELLIPTIC_H
#define ELLIPTIC_H

#include "pen.h"
#include "point.h"

class elliptic : public point
{
public:
    elliptic();

    bool setRadius1(int val);
    bool setRadius2(int val);
    int getRadius1();
    int getRadius2();

    pen rectPen;

private:
    int radius1;
    int radius2;
};

#endif // ELLIPTIC_H
