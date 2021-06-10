#include "arc.h"

Arc::Arc()
{
    // Конструктор
    type = PrimitiveTypes::ArcT;
    pen = new Pen();
    startAngle = 0;
    spanAngle = 0;
}

bool Arc::setStartAngle(int angle)
{
    // Установка начального угла
    if (angle < 0 || angle > INT32_MAX) return false;
    startAngle = angle;
    return true;
}

bool Arc::setSpanAngle(int angle)
{
    // Установка угла дуги
    if (angle < 0 || angle > INT32_MAX) return false;
    spanAngle = angle;
    return true;
}

int Arc::getStartAngle()
{
    // Возвращение начального угла
    return startAngle;
}

int Arc::getSpanAngle()
{
    // Возвращение угла дуги
    return spanAngle;
}

void Arc::draw(QImage * im)
{
    // Рисование дуги
    QPainter painter(im);

    int r, g, b, a;
    pen->getRGBA(r, g, b, a);
    QPen qpen(QColor(r, g, b, a));
    qpen.setWidth(this->pen->getWeight());
    qpen.setStyle(Qt::PenStyle(this->pen->getStyle()));

    painter.setPen(qpen);
    painter.drawArc(getX() - getRadius1(), getY() - getRadius2(), getRadius1() * 2, getRadius2() * 2, getStartAngle() * 16, getSpanAngle() * 16);
}
