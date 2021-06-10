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

    void createImage();
    QImage image;

protected:
    void paintEvent(QPaintEvent *);
    bool imCreated = false;

signals:

public slots:
};

#endif // CANVAS_H
