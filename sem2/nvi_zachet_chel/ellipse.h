#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QImage>
#include <QPainter>

#include "elliptic.h"

class Ellipse : public Elliptic
{
public:
    Ellipse();

    void draw(QImage &im);
};

#endif // ELLIPSE_H
