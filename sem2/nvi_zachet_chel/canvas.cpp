#include "canvas.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    // Конструктор
}

void Canvas::paintEvent(QPaintEvent *)
{
    // Событие при обновлении виджета
    if (imCreated)
    {
        QPainter painter(this);
        painter.drawImage(0, 0, image);
    }
}

void Canvas::createImage()
{
    // Метод для создания изображения
    image = QImage(width(), height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(0);
    imCreated = true;
}
