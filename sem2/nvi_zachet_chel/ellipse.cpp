#include "ellipse.h"

/// Конструктор
Ellipse::Ellipse()
{

}

/// Рисование эллипса
void Ellipse::draw(QImage &im)
{
    QPainter painter(&im);

    QPen pen(QColor(rectPen.getRed(), rectPen.getGreen(), rectPen.getBlue(), rectPen.getAlpha()));
    pen.setWidth(rectPen.getWidth());
    pen.setStyle(Qt::PenStyle(rectPen.getStyle()));

    painter.setPen(pen);
    painter.drawEllipse(getX() - getRadius1(), getY() - getRadius2(), getRadius1() * 2, getRadius2() * 2);
}
