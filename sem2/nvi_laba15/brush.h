#ifndef BRUSH_H
#define BRUSH_H

#include <color.h>


class Brush: public Color
{
public:
    Brush();

    bool setStyle(int s);
    int getStyle();

private:
    int style;
};

#endif // BRUSH_H
