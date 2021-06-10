#ifndef PIXEL_H
#define PIXEL_H

#include <QImage>
#include <QPainter>

#include "point.h"
#include "pen.h"


class pixel : public pen, public point
{
public:
    pixel();

    void draw(QImage &im);
};

#endif // PIXEL_H
