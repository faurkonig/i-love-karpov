#include "line.h"

/// Конструктор
Line::Line()
{

}

/// Рисование линии
void Line::draw(QImage &im)
{
    QPainter painter(&im);

    QPen pen(QColor(getRed(), getGreen(), getBlue(), getAlpha()));
    pen.setWidth(getWidth());
    pen.setStyle(Qt::PenStyle(getStyle()));

    painter.setPen(pen);
    painter.drawLine(p1.getX(), p1.getY(), p2.getX(), p2.getY());
}
