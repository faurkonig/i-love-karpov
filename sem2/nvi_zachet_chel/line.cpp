#include "line.h"

line::line()
{
    // Конструктор
}

void line::draw(QImage &im)
{
    // Рисование линии
    QPainter painter(&im);

    QPen pen(QColor(getRed(), getGreen(), getBlue(), getAlpha()));
    pen.setWidth(getWidth());
    pen.setStyle(Qt::PenStyle(getStyle()));

    painter.setPen(pen);
    painter.drawLine(p1.getX(), p1.getY(), p2.getX(), p2.getY());
}
