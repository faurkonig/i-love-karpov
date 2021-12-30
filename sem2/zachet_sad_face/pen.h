#ifndef PEN_H
#define PEN_H

#include "color.h"


class Pen : public Color
{
public:
    Pen();

    bool setWidth(int w);
    bool setStyle(int s);
    int getWidth();
    int getStyle();

private:
    int width;
    int style;
};

#endif // PEN_H
