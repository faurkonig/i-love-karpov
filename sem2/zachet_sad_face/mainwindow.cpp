#include "mainwindow.h"
#include "ui_mainwindow.h"

/// Конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->createImage(ui->widget->width(),ui->widget->height());

}

/// Деструктор
MainWindow::~MainWindow()
{
    delete ui;
}

/// Нажатие кнопки для рисования точки
void MainWindow::on_pushButtonPixel_clicked()
{
    ToolDialog *tool = new ToolDialog(this);
    int x, y;
    Pen p;
    tool->tpen = &p;
    if (tool->setPixel(&x, &y))
    {
        Pixel fig;
        fig.setRGBA(p.getRed(), p.getGreen(), p.getBlue(), p.getAlpha());
        fig.setStyle(p.getStyle());
        fig.setWidth(p.getWidth());
        fig.setXY(x, y);

        fig.draw(ui->widget->im);
        repaint();
    }
}

/// Нажатие кнопки для рисования линии
void MainWindow::on_pushButtonLine_clicked()
{
    ToolDialog *tool = new ToolDialog(this);
    int x1, y1, x2, y2;
    Pen p;
    tool->tpen = &p;
    if (tool->setLine(&x1, &y1, &x2, &y2))
    {
        Line fig;
        fig.setRGBA(p.getRed(), p.getGreen(), p.getBlue(), p.getAlpha());
        fig.setStyle(p.getStyle());
        fig.setWidth(p.getWidth());

        fig.p1.setXY(x1, y1);
        fig.p2.setXY(x2, y2);

        fig.draw(ui->widget->im);
        repaint();
    }
}

/// Нажатие кнопки для рисования прямоугольника
void MainWindow::on_pushButtonRect_clicked()
{
    ToolDialog *tool = new ToolDialog(this);
    int x1, y1, x2, y2;
    Pen p;
    Brush b;
    tool->tpen = &p;
    tool->tbrush = &b;
    if (tool->setRect(&x1, &y1, &x2, &y2))
    {
        Rectangle fig;
        fig.rectPen = p;
        fig.rectBrush = b;

        fig.p1.setXY(x1, y1);
        fig.p2.setXY(x2, y2);

        fig.draw(ui->widget->im);
        repaint();
    }
}

/// Нажатие кнопки для рисования эллипса
void MainWindow::on_pushButtonEllips_clicked()
{
    ToolDialog *tool = new ToolDialog(this);
    int x, y, r1, r2;
    Pen p;
    Brush b;
    tool->tpen = &p;
    tool->tbrush = &b;
    if (tool->setEllips(&x, &y, &r1, &r2))
    {
        Ellipse fig;
        fig.rectPen = p;
        fig.rectBrush = b;

        fig.setXY(x, y);
        fig.setRadius1(r1);
        fig.setRadius2(r2);

        fig.draw(ui->widget->im);
        repaint();
    }
}

/// Нажатие кнопки для рисования дуги
void MainWindow::on_pushButtonArc_clicked()
{
    ToolDialog *tool = new ToolDialog(this);
    int x, y, w, h, a1, a2;
    Pen p;
    tool->tpen = &p;
    if (tool->setArc(&x, &y, &w, &h, &a1, &a2))
    {
        Arc fig;
        fig.rectPen = p;

        fig.setXY(x, y);
        fig.setRadius1(w);
        fig.setRadius2(h);
        fig.setStartAngle(a1);
        fig.setSpanAngle(a2);

        fig.draw(ui->widget->im);
        repaint();
    }
}

/// Нажатие кнопки для рисования ломанной
void MainWindow::on_pushButtonCurve_clicked()
{
    CurveDialog *dialog = new CurveDialog(this);

    Point *points = nullptr;
    dialog->pointerToPoints = &points;
    Pen p;
    dialog->tpen = &p;
    int count;

    if (dialog->setCurve(&count))
    {
        Curve fig;
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
}

/// Нажатие кнопки для рисования многоугольника
void MainWindow::on_pushButtonPolygon_clicked()
{
    CurveDialog *dialog = new CurveDialog(this);

    Point *points = nullptr;
    dialog->pointerToPoints = &points;
    Pen p;
    dialog->tpen = &p;
    Brush b;
    dialog->tbrush = &b;
    int count;

    if (dialog->setPolygon(&count))
    {
        Polygon fig;
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

/// Нажатие на кнопки для рисования многоугольника
void MainWindow::on_pushButtonSadFace_clicked()
{
    ToolDialog *tool = new ToolDialog(this);
    int x, y, r1, r2;
    Pen p;
    tool->tpen = &p;
    if (tool->setFace(&x, &y, &r1, &r2))
    {
        SadFace fig;
        fig.rectPen = p;

        fig.setXY(x, y);
        fig.setRadius1(r1);
        fig.setRadius2(r2);

        fig.draw(ui->widget->im);
        repaint();
    }
}

/// Очищение изображения
void MainWindow::on_pushButtonClear_clicked()
{
    ui->widget->im.fill(0);
    repaint();
}
