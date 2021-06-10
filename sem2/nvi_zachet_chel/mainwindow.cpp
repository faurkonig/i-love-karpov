#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Конструктор
    ui->setupUi(this);

    ui->widget->createImage();

    ui->labelError->hide();
}

MainWindow::~MainWindow()
{
    // Деструктор
    delete ui;
}

void MainWindow::on_pushButtonDraw_clicked()
{
    // Обработка рисования
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
