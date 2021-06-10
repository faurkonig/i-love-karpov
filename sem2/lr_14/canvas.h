#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QImage>
#include <QPainter>


class canvas : public QWidget
{
    Q_OBJECT
public:
    explicit canvas(QWidget *parent = nullptr);

    QImage im;
    void createImage(int width, int height);

private:
    bool imCreated;

protected:
    void paintEvent(QPaintEvent *);

signals:

};

#endif // CANVAS_H
