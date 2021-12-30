#include "arc.h"

/// Конструктор
Arc::Arc()
{
    startAngle = 0;
    spanAngle = 0;
}

/// Установка начального угла
bool Arc::setStartAngle(int angle)
{
    if (angle < -360 || angle > 360) return false;
    startAngle = angle;
    return true;
}

/// Установка угла дуги
bool Arc::setSpanAngle(int angle)
{
    if (angle < -360 || angle > 360) return false;
    spanAngle = angle;
    return true;
}

/// Возвращение начального угла
int Arc::getStartAngle()
{
    return startAngle;
}

/// Возвращение угла дуги
int Arc::getSpanAngle()
{
    return spanAngle;
}

/// Рисование дуги
void Arc::draw(QImage &im)
{
    QPainter painter(&im);

    QPen pen(QColor(rectPen.getRed(), rectPen.getGreen(), rectPen.getBlue(), rectPen.getAlpha()));
    pen.setWidth(rectPen.getWidth());
    pen.setStyle(Qt::PenStyle(rectPen.getStyle()));

    painter.setPen(pen);
    painter.drawArc(getX() - getRadius1(), getY() - getRadius2(), getRadius1() * 2, getRadius2() * 2, getStartAngle() * 16, getSpanAngle() * 16);
}
