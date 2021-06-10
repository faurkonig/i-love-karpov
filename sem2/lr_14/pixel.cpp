#include "pixel.h"

pixel::pixel()
{
    // Конструктор
}

void pixel::draw(QImage &im)
{
    // Рисование пикселя
    QPainter painter(&im);

    QPen pen(QColor(getRed(), getGreen(), getBlue(), getAlpha()));
    pen.setWidth(getWidth());

    painter.setPen(pen);
    painter.drawPoint(getX(), getY());
}
