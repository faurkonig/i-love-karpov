#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Конструктор
    ui->setupUi(this);
    ui->widget->createImage(ui->widget->width(),ui->widget->height());

}

MainWindow::~MainWindow()
{
    // Деструктор
    delete ui;
}

void MainWindow::on_pushButtonDraw_clicked()
{
    int type = ui->comboBox->currentIndex();
    switch (type) {
    // Пиксель
    case 0: {
        ToolDialog *tool = new ToolDialog(this);
        int x, y;
        pen p;
        tool->tpen = &p;
        if (tool->setPixel(&x, &y))
        {
            pixel fig;
            fig.setRGBA(p.getRed(), p.getGreen(), p.getBlue(), p.getAlpha());
            fig.setStyle(p.getStyle());
            fig.setWidth(p.getWidth());
            fig.setXY(x, y);

            fig.draw(ui->widget->im);
            repaint();
        }
        break;
    }

    // Линия
    case 1: {
        ToolDialog *tool = new ToolDialog(this);
        int x1, y1, x2, y2;
        pen p;
        tool->tpen = &p;
        if (tool->setLine(&x1, &y1, &x2, &y2))
        {
            line fig;
            fig.setRGBA(p.getRed(), p.getGreen(), p.getBlue(), p.getAlpha());
            fig.setStyle(p.getStyle());
            fig.setWidth(p.getWidth());

            fig.p1.setXY(x1, y1);
            fig.p2.setXY(x2, y2);

            fig.draw(ui->widget->im);
            repaint();
        }
        break;
    }

    // Прямоугольник
    case 2: {
        ToolDialog *tool = new ToolDialog(this);
        int x1, y1, x2, y2;
        pen p;
        brush b;
        tool->tpen = &p;
        tool->tbrush = &b;
        if (tool->setRect(&x1, &y1, &x2, &y2))
        {
            rectangle fig;
            fig.rectPen = p;
            fig.rectBrush = b;

            fig.p1.setXY(x1, y1);
            fig.p2.setXY(x2, y2);

            fig.draw(ui->widget->im);
            repaint();
        }
        break;
    }

    // Эллипс
    case 3: {
        ToolDialog *tool = new ToolDialog(this);
        int x, y, r1, r2;
        pen p;
        brush b;
        tool->tpen = &p;
        tool->tbrush = &b;
        if (tool->setEllips(&x, &y, &r1, &r2))
        {
            ellipse fig;
            fig.rectPen = p;
            fig.rectBrush = b;

            fig.setXY(x, y);
            fig.setRadius1(r1);
            fig.setRadius2(r2);

            fig.draw(ui->widget->im);
            repaint();
        }
        break;
    }

    // Дуга
    case 4: {
        ToolDialog *tool = new ToolDialog(this);
        int x, y, w, h, a1, a2;
        pen p;
        tool->tpen = &p;
        if (tool->setArc(&x, &y, &w, &h, &a1, &a2))
        {
            arc fig;
            fig.rectPen = p;

            fig.setXY(x, y);
            fig.setRadius1(w);
            fig.setRadius2(h);
            fig.setStartAngle(a1);
            fig.setSpanAngle(a2);

            fig.draw(ui->widget->im);
            repaint();
        }
        break;
    }

    // Ломаная
    case 5: {
        CurveDialog *dialog = new CurveDialog(this);

        point *points = nullptr;
        dialog->pointerToPoints = &points;
        pen p;
        dialog->tpen = &p;
        int count;

        if (dialog->setCurve(&count))
        {
            curve fig;
            fig.pointerToPoints = &points;

            int rr, gg, bb, aa;
            p.getRGBA(rr, gg, bb, aa);
            fig.setRGBA(rr, gg, bb, aa);
            fig.setStyle(p.getStyle());
            fig.setWidth(p.getWidth());

            fig.draw(ui->widget->im, count);
            repaint();

            delete [] points;
            points = nullptr;
        }
        break;
    }

    // Многоугольник
    case 6: {
        CurveDialog *dialog = new CurveDialog(this);

        point *points = nullptr;
        dialog->pointerToPoints = &points;
        pen p;
        dialog->tpen = &p;
        brush b;
        dialog->tbrush = &b;
        int count;

        if (dialog->setPolygon(&count))
        {
            polygon fig;
            fig.pointerToPoints = &points;

            int rr, gg, bb, aa;
            p.getRGBA(rr, gg, bb, aa);
            fig.polyPen.setRGBA(rr, gg, bb, aa);
            fig.polyPen.setStyle(p.getStyle());
            fig.polyPen.setWidth(p.getWidth());

            b.getRGBA(rr, gg, bb, aa);
            fig.polyBrush.setRGBA(rr, gg, bb, aa);
            fig.polyBrush.setStyle(b.getStyle());

            fig.draw(ui->widget->im, count);
            repaint();

            delete [] points;
            points = nullptr;
        }
    }
    }
}

void MainWindow::on_pushButtonClear_clicked()
{
    // Очищение изображения
    ui->widget->im.fill(0);
    repaint();
}

