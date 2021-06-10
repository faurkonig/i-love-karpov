#include "ellipse.h"

Ellipse::Ellipse()
{
     // Конструктор
    type = PrimitiveTypes::EllipseT;
    pen = new Pen();
    brush = new Brush();
}

void Ellipse::draw(QImage * im)
{
    // Рисование эллипса
    QPainter painter(im);

    int r, g, b, a;
    pen->getRGBA(r, g, b, a);
    QPen qpen(QColor(r, g, b, a));
    qpen.setWidth(pen->getWeight());
    qpen.setStyle(Qt::PenStyle(pen->getStyle()));

    brush->getRGBA(r, g, b, a);
    QBrush qbrush(QColor(r, g, b, a));
    qbrush.setStyle(Qt::BrushStyle(brush->getStyle()));

    painter.setPen(qpen);
    painter.setBrush(qbrush);
    painter.drawEllipse(getX() - getRadius1(), getY() - getRadius2(), getRadius1() * 2, getRadius2() * 2);
}
