#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QPainter painter(&ui->canvasWidget->image);
    ui->canvasWidget->image.fill(0);

    bool flag;
    int starCount = ui->lineEdit->text().toInt(&flag);
    if (flag && starCount >= 0)
    {
        painter.fillRect(0, 0, 500, 500, QBrush(QColor(0, 14, 41))); // Небо

        // Позиция центра звезды и её "радиус"
        int starX, starY, starRandWidth, starWidth  = 2;
        QPen pen = QPen(Qt::yellow); // Карандаш для рисования
        pen.setWidth(1);
        painter.setPen(pen); // Устанавливаем карандаш
        for (int i = 0; i < starCount; i++)
        {
            // Получаем случайные координаты центра цвезды
            starX = rand() % 500; // По всей ширине экрана
            starY = rand() % 450; // До земли - радиус звезды
            starRandWidth = rand() % 3;

            int tempWidth = (starRandWidth == 1) ? starWidth + 1 :
                                (starRandWidth == 2) ? starWidth - 1:
                                    starWidth;

            if ((starY > 388 && starX >= 50 && starX <= 450)
                    || (starY <= 388 && starY >= 345 &&
                    43 * starX + 15 * starY >= 7325 && 43 * starX - 15 * starY <= 14175))
            {
                i--;
                continue;
            }

            // Рисуем шестиконечную звезду как 2 треугольника - 6 линий
            // Треугольник, направленный вниз
            painter.drawLine(starX - tempWidth, starY - tempWidth / 2, starX + tempWidth, starY - tempWidth / 2); // основание
            painter.drawLine(starX + tempWidth, starY - tempWidth / 2, starX, starY + tempWidth);
            painter.drawLine(starX, starY + tempWidth, starX - tempWidth, starY - tempWidth / 2);
            // Треугольник, направленный вверх
            painter.drawLine(starX - tempWidth, starY + tempWidth / 2, starX + tempWidth, starY + tempWidth / 2); // основание
            painter.drawLine(starX + tempWidth, starY + tempWidth / 2, starX, starY - tempWidth);
            painter.drawLine(starX, starY - tempWidth, starX - tempWidth, starY + tempWidth / 2);
        }

        painter.fillRect(0, 450, 500, 50, QBrush(QColor(37, 93, 19))); // Земля

        QColor moonClr(252, 240, 207); // Цвет луны
        QColor darkMoonClr(212, 205, 186); // Цвет кратеров луны
        // Устанавливаем цвета
        painter.setBrush(QBrush(moonClr));
        painter.setPen(QPen(moonClr));
        painter.drawEllipse(400, 100, 50, 50);
        //
        painter.setBrush(QBrush(darkMoonClr));
        painter.setPen(QPen(darkMoonClr));
        painter.drawEllipse(410, 110, 10, 10);
        painter.drawEllipse(425, 105, 12, 12);
        painter.drawEllipse(405, 127, 8, 8);
        painter.drawEllipse(430, 120, 11, 11);
        painter.drawEllipse(415, 130, 15, 15);

        painter.fillRect(50, 360, 400, 90, QBrush(QColor(126, 126, 93))); // Основа дома
        painter.fillRect(230, 405, 30, 45, QBrush(QColor(130, 114, 33))); // Дверь

        painter.fillRect(232, 428, 7, 3, QBrush(QColor(145, 145, 145))); // Ручка двери

        // Окна левой стороны
        painter.fillRect(60, 400, 17, 26, QBrush(QColor(200, 200, 200)));
        painter.fillRect(92, 400, 17, 26, QBrush(QColor(200, 200, 200)));
        painter.fillRect(124, 400, 17, 26, QBrush(QColor(253, 249, 119)));
        painter.fillRect(157, 400, 17, 26, QBrush(QColor(200, 200, 200)));
        // Окна правой стороны
        painter.fillRect(60 + 265, 400, 17, 26, QBrush(QColor(200, 200, 200)));
        painter.fillRect(92 + 265, 400, 17, 26, QBrush(QColor(200, 200, 200)));
        painter.fillRect(124 + 265, 400, 17, 26, QBrush(QColor(200, 200, 200)));
        painter.fillRect(157 + 265, 400, 17, 26, QBrush(QColor(200, 200, 200)));

        // Построение пути для контура крыши
        QPainterPath roofPath;
        roofPath.moveTo(50, 345);
        roofPath.lineTo(450, 345);
        roofPath.lineTo(465, 388);
        roofPath.lineTo(35, 388);
        roofPath.lineTo(50, 345);
        painter.fillPath(roofPath, QBrush(QColor(60, 60, 60))); // Заливка крыши по пути

        // Колонны левой стороны
        painter.fillRect(190, 388, 7, 62, QBrush(QColor(150, 150, 105)));
        painter.fillRect(205, 388, 7, 62, QBrush(QColor(150, 150, 105)));
        // Колонны правой стороны
        painter.fillRect(190 + 90, 388, 7, 62, QBrush(QColor(150, 150, 105)));
        painter.fillRect(205 + 90, 388, 7, 62, QBrush(QColor(150, 150, 105)));
    }
    else
    {
        // Выводим сообщение об ошибке на холст
        painter.setPen(QPen(Qt::red));
        painter.drawText(50, 50, "Ошибка");
    }

    repaint(); // Перерисовываем
}
