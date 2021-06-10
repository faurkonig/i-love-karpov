#include "line.h"

Line::Line()
{
    // Конструктор
    type = PrimitiveTypes::LineT;
    pen = new Pen();
}

void Line::draw(QImage * im)
{
    // Рисование линии
    QPainter painter(im);

    int r, g, b, a;
    pen->getRGBA(r, g, b, a);
    QPen qpen(QColor(r, g, b, a));
    qpen.setWidth(this->pen->getWeight());
    qpen.setStyle(Qt::PenStyle(pen->getStyle()));

    painter.setPen(qpen);
    painter.drawLine(p1.getX(), p1.getY(), p2.getX(), p2.getY());
}
