#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QImage>
#include <QPainter>

#include "elliptic.h"
#include "brush.h"

class ellipse : public elliptic
{
public:
    ellipse();

    brush rectBrush;

    void draw(QImage &im);
};

#endif // ELLIPSE_H
