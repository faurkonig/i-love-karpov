#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QImage>
#include <QPainter>


class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    QImage image;
    void createImage(int width, int height);

private:
    bool imCreated;

protected:
    void paintEvent(QPaintEvent *);

signals:

};

#endif // CANVAS_H
