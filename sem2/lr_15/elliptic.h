#ifndef ELLIPTIC_H
#define ELLIPTIC_H

#include <point.h>

#include <consts.h>


class Elliptic : public Point
{
public:
    Elliptic();

    bool setRadius1(int val);
    bool setRadius2(int val);
    int getRadius1();
    int getRadius2();

private:
    int radius1;
    int radius2;
};

#endif // ELLIPTIC_H
