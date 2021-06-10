#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QImage>
#include <QPainter>

#include "elliptic.h"

class ellipse : public elliptic
{
public:
    ellipse();

    void draw(QImage &im);
};

#endif // ELLIPSE_H
