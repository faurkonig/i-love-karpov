#ifndef PEN_H
#define PEN_H

#include <color.h>


class Pen : public Color
{
public:
    Pen();

    bool setWeight(int w);
    bool setStyle(int s);
    int getWeight();
    int getStyle();

private:
    int weight;
    int style;
};

#endif // PEN_H
