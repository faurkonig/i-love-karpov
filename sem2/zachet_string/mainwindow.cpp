#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "charcounter.h"

/// Конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

/// Деструктор
MainWindow::~MainWindow()
{
    delete ui;
}

/// Получение и вывод результата
void MainWindow::on_pushButtonResult_clicked()
{
    // Добавление ввода в класс + проверка ввода
    CharCounter counter;
    bool okStr = counter.setString(ui->lineEditStr->text());
    bool okChar = counter.setCharacter(ui->lineEditChar->text());

    if (okStr && okChar)
    {
        // Если ввод правильный
        ui->labelResult->setText("Результат: "+QString::number(counter.getResult()));
    }
    else
    {
        // Если неправильный
        ui->labelResult->setText("Неверные данные");
    }
}
