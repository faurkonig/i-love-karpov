#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    image = QImage(500,500,QImage::Format_ARGB32_Premultiplied);
}

void Canvas::paintEvent(QPaintEvent*)
{
    QPainter painter(this);

    painter.drawImage(0,0,image);
}
