#include "canvas.h"

/// Конструктор
Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    imCreated = false;
}

/// Событие при обновлении виджета
void Canvas::paintEvent(QPaintEvent *)
{
    if (imCreated)
    {
        QPainter painter(this);

        painter.drawImage(0, 0, image);
    }
}

/// Создание изображения
void Canvas::createImage(int width, int height)
{
    image = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    image.fill(0);
    imCreated = true;
}
