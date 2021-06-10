#ifndef ARC_H
#define ARC_H

#include <QImage>
#include <QPainter>

#include "elliptic.h"


class arc : public elliptic
{
public:
    arc();

    bool setStartAngle(int angle);
    bool setSpanAngle(int angle);
    int getStartAngle();
    int getSpanAngle();

    void draw(QImage &im);

private:
    int startAngle;
    int spanAngle;
};

#endif // ARC_H
