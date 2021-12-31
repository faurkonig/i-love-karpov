#include "human.h"

/// Конструктор
Human::Human()
{
    height = 1;
    width = 1;
    faceWidth = 1;
}

/// Указание высоты
bool Human::setHeight(int value)
{
    if (value < 20 || value > 300) return false;
    height = value;
    return true;
}

/// Указание ширины
bool Human::setWidth(int value)
{
    if (value < 5 || value > 100) return false;
    width = value;
    return true;
}

/// Указание размера лица
bool Human::setFaceWidth(int value)
{
    if (value < 10 || value > 50) return false;
    faceWidth = value;
    return true;
}

/// Возвращение высоты
int Human::getHeight()
{
    return height;
}

/// Возвращение ширины
int Human::getWidth()
{
    return width;
}

/// Возвращение размера лица
int Human::getFaceWidth()
{
    return faceWidth;
}

/// Рисование человечка
void Human::draw(QImage * im)
{
    Pen mainPen;
    mainPen.setWidth(5);

    // Некоторые изначальные вычисления
    int canvasWidth = im->width();
    int canvasHeight = im->height();
    int legsTopHeight = canvasHeight - 10 - (height / 3);
    int bodyTopHeight = legsTopHeight - (height * 2 / 3);
    int headTopHeight = bodyTopHeight - faceWidth;
    int hei3 = height / 3;
    int fac3 = faceWidth / 3;

    // Объявление классов и передача карандаша
    Line l1, l2, l3, l4;
    Ellipse body;
    Ellipse head;
    Arc smile;
    Pixel p1;
    Pixel p2;
    l1.setWidth(mainPen.getWidth());
    l2.setWidth(mainPen.getWidth());
    l3.setWidth(mainPen.getWidth());
    l4.setWidth(mainPen.getWidth());
    body.rectPen = mainPen;
    head.rectPen = mainPen;
    smile.rectPen = mainPen;
    p1.setWidth(mainPen.getWidth());
    p2.setWidth(mainPen.getWidth());

    // Ноги
    l1.p1.setXY(canvasWidth / 2, legsTopHeight);
    l1.p2.setXY((canvasWidth / 2) - hei3, canvasHeight - 10);
    l2.p1.setXY(canvasWidth / 2, legsTopHeight);
    l2.p2.setXY((canvasWidth / 2) + hei3, canvasHeight - 10);

    // Руки
    l3.p1.setXY(canvasWidth / 2, bodyTopHeight);
    l3.p2.setXY((canvasWidth / 2) - (hei3 * 1.5), bodyTopHeight);
    l4.p1.setXY(canvasWidth / 2, bodyTopHeight);
    l4.p2.setXY((canvasWidth / 2) + (hei3 * 1.5), bodyTopHeight);

    // Тело
    body.setXY(canvasWidth / 2, bodyTopHeight + hei3);
    body.setRadius1(width);
    body.setRadius2(hei3);

    // Голова
    head.setXY(canvasWidth / 2, headTopHeight);
    head.setRadius1(faceWidth);
    head.setRadius2(faceWidth);

    // Улыбка
    smile.setXY(canvasWidth / 2, headTopHeight);
    smile.setRadius1(faceWidth * 0.6);
    smile.setRadius2(faceWidth * 0.6);
    smile.setStartAngle(180);
    smile.setSpanAngle(180);

    // Глаза
    p1.setXY(canvasWidth / 2 - fac3, headTopHeight - fac3);
    p2.setXY(canvasWidth / 2 + fac3, headTopHeight - fac3);

    // Рисование всех примитивов
    l1.draw(*im);
    l2.draw(*im);
    l3.draw(*im);
    l4.draw(*im);
    body.draw(*im);
    head.draw(*im);
    smile.draw(*im);
    p1.draw(*im);
    p2.draw(*im);
}
