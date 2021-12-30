#ifndef SADFACE_H
#define SADFACE_H

#include <QImage>
#include "ellipse.h"
#include "arc.h"


class SadFace: public Elliptic
{
public:
    SadFace();

    void draw(QImage &im);
};

#endif // SADFACE_H
