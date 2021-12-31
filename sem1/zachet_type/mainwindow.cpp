#include "mainwindow.h"
#include "ui_mainwindow.h"

/// Конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPalette defaultColor = this->palette();
    intColor = defaultColor;
    intColor.setColor(QPalette::Window, QColor(0xFF4980CA));
    floatColor = defaultColor;
    floatColor.setColor(QPalette::Window, QColor(0xFFA449CA));
    nonNumColor = defaultColor;
    nonNumColor.setColor(QPalette::Window, QColor(0xFFCA4987));
    emptyColor = defaultColor;
    emptyColor.setColor(QPalette::Window, QColor(0xFFCA6B49));
}

/// Деструктор
MainWindow::~MainWindow()
{
    delete ui;
}

/// Обработчик изменений в поле ввода
void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    if (arg1.isEmpty())
    {
        // Если строка пустая
        this->setPalette(emptyColor);
        ui->labelResult->setText("пустая строка");
    }
    else
    {
        bool isInt;
        arg1.toInt(&isInt);
        if (isInt)
        {
            // Если введено целое число
            this->setPalette(intColor);
            ui->labelResult->setText("целое число");
        }
        else
        {
            bool isDouble;
            arg1.toDouble(&isDouble);
            if (isDouble)
            {
                // Если введено вечественное число
                this->setPalette(floatColor);
                ui->labelResult->setText("вещественное число");
            }
            else
            {
                // Если введено не число
                this->setPalette(nonNumColor);
                ui->labelResult->setText("не число");
            }
        }
    }
}
