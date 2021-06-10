#include "curve.h"

curve::curve()
{
    // Конструктор
}

void curve::draw(QImage &im, int n)
{
    // Рисование ломанной
    QPainter painter(&im);

    QPainterPath path;

    int cx, cy;
    (*pointerToPoints)[0].getXY(cx, cy);
    path.moveTo(cx, cy);
    for (int i = 1; i < n; i++)
    {
        (*pointerToPoints)[i].getXY(cx, cy);
        path.lineTo(cx, cy);
    }

    QPen pen(QColor(getRed(), getGreen(), getBlue(), getAlpha()));
    pen.setWidth(getWidth());
    pen.setStyle(Qt::PenStyle(getStyle()));

    painter.setPen(pen);
    painter.drawPath(path);
}
