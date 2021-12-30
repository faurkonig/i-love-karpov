#ifndef ARC_H
#define ARC_H

#include <QImage>
#include <QPainter>

#include "elliptic.h"
#include "brush.h"


class Arc : public Elliptic
{
public:
    Arc();

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
