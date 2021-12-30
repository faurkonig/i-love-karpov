#include "rectangle.h"

rectangle::rectangle()
{
    // Конструктор
}

void rectangle::draw(QImage &im)
{
    // Рисование прямоугольника
    QPainter painter(&im);

    QPen pen(QColor(rectPen.getRed(), rectPen.getGreen(), rectPen.getBlue(), rectPen.getAlpha()));
    pen.setWidth(rectPen.getWidth());
    pen.setStyle(Qt::PenStyle(rectPen.getStyle()));

    QBrush brush(QColor(rectBrush.getRed(), rectBrush.getGreen(), rectBrush.getBlue(), rectBrush.getAlpha()));
    brush.setStyle(Qt::BrushStyle(rectBrush.getStyle()));

    int width = p2.getX() - p1.getX();
    int height = p2.getY() - p1.getY();

    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(p1.getX(), p1.getY(), width, height);
}
