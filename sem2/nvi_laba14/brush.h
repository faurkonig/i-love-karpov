#ifndef BRUSH_H
#define BRUSH_H

#include "color.h"


class brush : public color
{
public:
    brush();

    bool setStyle(int s);
    int getStyle();

private:
    int style;
};

#endif // BRUSH_H
