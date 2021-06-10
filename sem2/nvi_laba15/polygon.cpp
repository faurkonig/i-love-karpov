#include "polygon.h"

Polygon::Polygon()
{
    // Конструктор
    type = PrimitiveTypes::PolygonT;
    pen = new Pen();
    brush = new Brush();
    points = new QVector<Point>(3);
}

void Polygon::draw(QImage * im)
{
    // Рисование многоугольника
    QPainter painter(im);

    QPainterPath path;

    int cx, cy;
    (*points)[0].getXY(cx, cy);
    path.moveTo(cx, cy);
    int count = points->count();
    for (int i = 1; i < count; i++)
    {
        (*points)[i].getXY(cx, cy);
        path.lineTo(cx, cy);
    }
    (*points)[0].getXY(cx, cy);
    path.lineTo(cx, cy);

    int r, g, b, a;
    this->pen->getRGBA(r, g, b, a);
    QPen qpen(QColor(r, g, b, a));
    qpen.setWidth(this->pen->getWeight());
    qpen.setStyle(Qt::PenStyle(this->pen->getStyle()));

    brush->getRGBA(r, g, b, a);
    QBrush qbrush(QColor(r, g, b, a));
    qbrush.setStyle(Qt::BrushStyle(brush->getStyle()));

    painter.setPen(qpen);
    painter.fillPath(path, qbrush);
    painter.drawPath(path);
}
