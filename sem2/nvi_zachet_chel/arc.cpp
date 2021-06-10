#include "arc.h"

arc::arc()
{
    // Конструктор
    startAngle = 0;
    spanAngle = 0;
}

bool arc::setStartAngle(int angle)
{
    // Установка начального угла
    if (angle < -360 || angle > 360) return false;
    startAngle = angle;
    return true;
}

bool arc::setSpanAngle(int angle)
{
    // Установка угла дуги
    if (angle < -360 || angle > 360) return false;
    spanAngle = angle;
    return true;
}

int arc::getStartAngle()
{
    // Возвращение начального угла
    return startAngle;
}

int arc::getSpanAngle()
{
    // Возвращение угла дуги
    return spanAngle;
}

void arc::draw(QImage &im)
{
    // Рисование дуги
    QPainter painter(&im);

    QPen pen(QColor(rectPen.getRed(), rectPen.getGreen(), rectPen.getBlue(), rectPen.getAlpha()));
    pen.setWidth(rectPen.getWidth());
    pen.setStyle(Qt::PenStyle(rectPen.getStyle()));

    painter.setPen(pen);
    painter.drawArc(getX() - getRadius1(), getY() - getRadius2(), getRadius1() * 2, getRadius2() * 2, getStartAngle() * 16, getSpanAngle() * 16);
}
