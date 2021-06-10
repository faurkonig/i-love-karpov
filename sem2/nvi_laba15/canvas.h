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

protected:
    void paintEvent(QPaintEvent *);

private:
    bool imCreated = false;
};

#endif // CANVAS_H
