#include "polygon.h"

polygon::polygon()
{
    // Конструктор
}

void polygon::draw(QImage &im, int n)
{
    // Рисование многоугольника
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
    (*pointerToPoints)[0].getXY(cx, cy);
    path.lineTo(cx, cy);

    QPen pen(QColor(polyPen.getRed(), polyPen.getGreen(), polyPen.getBlue(), polyPen.getAlpha()));
    pen.setWidth(polyPen.getWidth());
    pen.setStyle(Qt::PenStyle(polyPen.getStyle()));

    QBrush brush(QColor(polyBrush.getRed(), polyBrush.getGreen(), polyBrush.getBlue(), polyBrush.getAlpha()));
    brush.setStyle(Qt::BrushStyle(polyBrush.getStyle()));

    painter.setPen(pen);
    painter.fillPath(path, brush);
    painter.drawPath(path);
}
