#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>

#include <QtMath>

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    QImage image;

protected:
    void paintEvent(QPaintEvent *);

signals:

};

#endif // CANVAS_H
