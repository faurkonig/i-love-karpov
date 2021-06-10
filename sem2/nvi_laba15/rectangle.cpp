#include "rectangle.h"

Rectangle::Rectangle()
{
    // Конструктор
    type = PrimitiveTypes::RectangleT;
    pen = new Pen();
    brush = new Brush();
}

void Rectangle::draw(QImage * im)
{
    // Рисование прямоугольника
    QPainter painter(im);

    int r, g, b, a;
    pen->getRGBA(r, g, b, a);
    QPen qpen(QColor(r, g, b, a));
    qpen.setWidth(pen->getWeight());
    qpen.setStyle(Qt::PenStyle(pen->getStyle()));

    brush->getRGBA(r, g, b, a);
    QBrush qbrush(QColor(r, g, b, a));
    qbrush.setStyle(Qt::BrushStyle(brush->getStyle()));

    int width = p2.getX() - p1.getX();
    int height = p2.getY() - p1.getY();

    painter.setPen(qpen);
    painter.setBrush(qbrush);
    painter.drawRect(p1.getX(), p1.getY(), width, height);
}
