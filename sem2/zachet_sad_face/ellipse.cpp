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

    QBrush brush(QColor(rectBrush.getRed(), rectBrush.getGreen(), rectBrush.getBlue(), rectBrush.getAlpha()));
    brush.setStyle(Qt::BrushStyle(rectBrush.getStyle()));

    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawEllipse(getX() - getRadius1(), getY() - getRadius2(), getRadius1() * 2, getRadius2() * 2);
}
