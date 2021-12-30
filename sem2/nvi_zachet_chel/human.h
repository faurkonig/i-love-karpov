#ifndef HUMAN_H
#define HUMAN_H

#include <QImage>
#include <QPainter>
#include <ellipse.h>
#include <line.h>
#include <pixel.h>
#include <pen.h>
#include <arc.h>

class Human
{
public:
    Human();

    void draw(QImage * im);

    bool setHeight(int);
    bool setWidth(int);
    bool setFaceWidth(int);
    int getHeight();
    int getWidth();
    int getFaceWidth();

private:
    int height;
    int width;
    int faceWidth;
};

#endif // HUMAN_H
