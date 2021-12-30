#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QImage>
#include <QPainter>

#include "elliptic.h"
#include "brush.h"

class Ellipse : public Elliptic
{
public:
    Ellipse();

    Brush rectBrush;

    void draw(QImage &im);
};

#endif // ELLIPSE_H
