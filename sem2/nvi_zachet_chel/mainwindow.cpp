#include "mainwindow.h"
#include "ui_mainwindow.h"

/// Конструктор
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widget->createImage(ui->widget->width(), ui->widget->height());

    ui->labelError->hide();
}

/// Деструктор
MainWindow::~MainWindow()
{
    delete ui;
}

/// Обработка рисования
void MainWindow::on_pushButtonDraw_clicked()
{
    bool ok = true;
    ok = man.setHeight(ui->spinBoxHeight->value());
    if (ok) ok = man.setWidth(ui->spinBoxWidth->value());
    if (ok) ok = man.setFaceWidth(ui->spinBoxFaceWidth->value());

    if (ok)
    {
        ui->labelError->hide();
        ui->widget->image.fill(0);
        man.draw(&(ui->widget->image));
        repaint();
    }
    else
    {
        ui->labelError->show();
    }
}
