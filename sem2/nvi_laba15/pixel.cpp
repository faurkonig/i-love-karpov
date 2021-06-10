#include "pixel.h"

Pixel::Pixel()
{
    // Конструктор
    type = PrimitiveTypes::PixelT;
    pen = new Pen();
}

void Pixel::draw(QImage * im)
{
    // Рисование пикселя
    QPainter painter(im);

    int r, g, b, a;
    pen->getRGBA(r, g, b, a);
    QPen qpen(QColor(r, g, b, a));
    qpen.setWidth(pen->getWeight());

    painter.setPen(qpen);
    painter.drawPoint(getX(), getY());
}
