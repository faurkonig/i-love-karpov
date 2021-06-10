#include "canvas.h"

canvas::canvas(QWidget *parent) : QWidget(parent)
{
    // Конструктор
    imCreated = false;
}

void canvas::paintEvent(QPaintEvent *)
{
    // Событие при обновлении виджета
    if (imCreated)
    {
        QPainter painter(this);

        painter.drawImage(0, 0, im);
    }
}

void canvas::createImage(int width, int height)
{
    // Создание изображения
    im = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    im.fill(0);
    imCreated = true;
}
