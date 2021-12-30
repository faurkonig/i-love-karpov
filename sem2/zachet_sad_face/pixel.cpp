#include "pixel.h"

/// Конструктор
Pixel::Pixel()
{

}

/// Рисование пикселя
void Pixel::draw(QImage &im)
{
    QPainter painter(&im);

    QPen pen(QColor(getRed(), getGreen(), getBlue(), getAlpha()));
    pen.setWidth(getWidth());

    painter.setPen(pen);
    painter.drawPoint(getX(), getY());
}
