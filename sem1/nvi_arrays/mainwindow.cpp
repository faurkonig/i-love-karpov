#include "mainwindow.h"
#include "ui_mainwindow.h"

// Хранение фона для хранения
// Чтобы работало красиво не только в светлой теме
static QColor defaultBackgroundColor;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spinBox->setMaximum(MAX_ARR); // Установка максимального числа значений в спинбокс
    noAutoChange = true; // Переменная для "блокировки" onCellChanged в таблице
    // Нахождение "нормального" цвета для фона клеток таблицы
    defaultBackgroundColor = ui->tableWidget->palette().color(QPalette::Base);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_spinBox_valueChanged(int arg1)
{
    // Изменение кол-ва значений в таблице на значение из спинбокса
    ui->tableWidget->setRowCount(arg1);
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    if (noAutoChange)
    {
        // Обработка изменений в таблице
        bool ok;
        ui->tableWidget->item(row, column)->text().toInt(&ok);
        if (ok)
        {
            // Если введена нормальная целая цифра
            ui->tableWidget->item(row, column)->setBackground(defaultBackgroundColor);
        }
        else
        {
            // Если введено неправильное значение
            ui->tableWidget->item(row, column)->setBackground(Qt::red);
        }
    }

    // Очистка других полей (для красоты), хоть и функционально это не очень удобно иногда пожалуй
    ui->labelSum->clear();
    ui->labelMin->clear();
    ui->labelMax->clear();
    ui->labelAver->clear();
}

void MainWindow::on_pushButtonRandom_clicked()
{
    noAutoChange = false;

    // Создаём пустой массив с максимальным размером и узнаём нужный нам размер
    int arr[MAX_ARR];
    int arrSize = ui->spinBox->value();

    for (int i = 0; i < arrSize; i++)
    {
        // Формируем массив и в него вставляем случайное значение в нужном нам диапазоне
        arr[i] = rand() % (RAND_OFF + 1) - (RAND_OFF / 2);

        if (ui->tableWidget->item(i, 0) == nullptr)
        {
            // Если ячейки не существует, то она создаётся
            QTableWidgetItem * ti;
            ti = new QTableWidgetItem;
            ui->tableWidget->setItem(i, 0, ti);
        }
    }

    for (int i = 0; i < arrSize; i++)
    {
        // Вставляем в ячейки таблицы значения из массива и устанавливаем цвет ячеек на стандартный
        ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
        ui->tableWidget->item(i, 0)->setBackground(defaultBackgroundColor);
    }

    noAutoChange = true;

    // Очистка других полей (для красоты), хоть и функционально это не очень удобно иногда пожалуй
    ui->labelSum->clear();
    ui->labelMin->clear();
    ui->labelMax->clear();
    ui->labelAver->clear();
}

void MainWindow::on_pushButtonSum_clicked()
{
    noAutoChange = false;

    // Создаём пустой массив с максимальным размером и узнаём нужный нам размер
    int arr[MAX_ARR];
    int arrSize = ui->spinBox->value();

    // Флаги для проверки ячеек на корректность значений
    bool okArr = true, okCell = true;

    for (int i = 0; i < arrSize; i++)
    {
        // Формируем массив
        if (ui->tableWidget->item(i, 0) != nullptr)
        {
            // Если ячейка существует, то проверяем на корректность его значения
            arr[i] = ui->tableWidget->item(i, 0)->text().toInt(&okCell);
        }
        else
        {
            // Если ячейки не существует, то она создаётся
            okCell = false;
            QTableWidgetItem * ti;
            ti = new QTableWidgetItem;
            ui->tableWidget->setItem(i, 0, ti);
        }

        if (okCell) // Если ячейка правильная, ставим её нормальный цвет
            ui->tableWidget->item(i, 0)->setBackground(defaultBackgroundColor);
        else
        {
            // Если нет, то меняем флаг правильности массива и ставим для ячейки красный цвет
            okArr = false;
            ui->tableWidget->item(i, 0)->setBackground(Qt::red);
        }
    }

    int sum = 0;
    if (okArr)
    {
        // Если все значения в массиве правильные, то проходим по массиву и находим сумму
        for (int i = 0; i < arrSize; i++)
            sum += arr[i];
        ui->labelSum->setNum(sum);
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelSum->setText("Error");
    }

    noAutoChange = true;

    // Очистка других полей (для красоты), хоть и функционально это не очень удобно иногда пожалуй
    ui->labelMin->clear();
    ui->labelMax->clear();
    ui->labelAver->clear();
}

void MainWindow::on_pushButtonMin_clicked()
{
    noAutoChange = false;

    // Создаём пустой массив с максимальным размером и узнаём нужный нам размер
    int arr[MAX_ARR];
    int arrSize = ui->spinBox->value();

    // Флаги для проверки ячеек на корректность значений
    bool okArr = true, okCell = true;

    for (int i = 0; i < arrSize; i++)
    {
        // Формируем массив
        if (ui->tableWidget->item(i, 0) != nullptr)
        {
            // Если ячейка существует, то проверяем на корректность его значения
            arr[i] = ui->tableWidget->item(i, 0)->text().toInt(&okCell);
        }
        else
        {
            // Если ячейки не существует, то она создаётся
            okCell = false;
            QTableWidgetItem * ti;
            ti = new QTableWidgetItem;
            ui->tableWidget->setItem(i, 0, ti);
        }

        if (okCell) // Если ячейка правильная, ставим её нормальный цвет
            ui->tableWidget->item(i, 0)->setBackground(defaultBackgroundColor);
        else
        {
            // Если нет, то меняем флаг правильности массива и ставим для ячейки красный цвет
            okArr = false;
            ui->tableWidget->item(i, 0)->setBackground(Qt::red);
        }
    }

    // Для нахождения мин/макс значения можно принять первое попавшееся значение из массива за изначальное значение
    int min = arr[0];
    if (okArr)
    {
        // Если все значения в массиве правильные, то проходим по массиву и находим минимум
        for (int i = 1; i < arrSize; i++)
            if (arr[i] < min)
                min = arr[i];
        ui->labelMin->setNum(min);
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelMin->setText("Error");
    }

    noAutoChange = true;

    // Очистка других полей (для красоты), хоть и функционально это не очень удобно иногда пожалуй
    ui->labelSum->clear();
    ui->labelMax->clear();
    ui->labelAver->clear();
}

void MainWindow::on_pushButtonMax_clicked()
{
    noAutoChange = false;

    // Создаём пустой массив с максимальным размером и узнаём нужный нам размер
    int arr[MAX_ARR];
    int arrSize = ui->spinBox->value();

    // Флаги для проверки ячеек на корректность значений
    bool okArr = true, okCell = true;

    for (int i = 0; i < arrSize; i++)
    {
        // Формируем массив
        if (ui->tableWidget->item(i, 0) != nullptr)
        {
            // Если ячейка существует, то проверяем на корректность его значения
            arr[i] = ui->tableWidget->item(i, 0)->text().toInt(&okCell);
        }
        else
        {
            // Если ячейки не существует, то она создаётся
            okCell = false;
            QTableWidgetItem * ti;
            ti = new QTableWidgetItem;
            ui->tableWidget->setItem(i, 0, ti);
        }

        if (okCell) // Если ячейка правильная, ставим её нормальный цвет
            ui->tableWidget->item(i, 0)->setBackground(defaultBackgroundColor);
        else
        {
            // Если нет, то меняем флаг правильности массива и ставим для ячейки красный цвет
            okArr = false;
            ui->tableWidget->item(i, 0)->setBackground(Qt::red);
        }
    }

    // Для нахождения мин/макс значения можно принять первое попавшееся значение из массива за изначальное значение
    int max = arr[0];
    if (okArr)
    {
        // Если все значения в массиве правильные, то проходим по массиву и находим максимум
        for (int i = 0; i < arrSize; i++)
            if (arr[i] > max)
                max = arr[i];
        ui->labelMax->setNum(max);
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelMax->setText("Error");
    }

    noAutoChange = true;

    // Очистка других полей (для красоты), хоть и функционально это не очень удобно иногда пожалуй
    ui->labelMin->clear();
    ui->labelSum->clear();
    ui->labelAver->clear();
}

void MainWindow::on_pushButtonAverage_clicked()
{
    noAutoChange = false;

    // Создаём пустой массив с максимальным размером и узнаём нужный нам размер
    int arr[MAX_ARR];
    int arrSize = ui->spinBox->value();

    // Флаги для проверки ячеек на корректность значений
    bool okArr = true, okCell = true;

    for (int i = 0; i < arrSize; i++)
    {
        // Формируем массив
        if (ui->tableWidget->item(i, 0) != nullptr)
        {
            // Если ячейка существует, то проверяем на корректность его значения
            arr[i] = ui->tableWidget->item(i, 0)->text().toInt(&okCell);
        }
        else
        {
            // Если ячейки не существует, то она создаётся
            okCell = false;
            QTableWidgetItem * ti;
            ti = new QTableWidgetItem;
            ui->tableWidget->setItem(i, 0, ti);
        }

        if (okCell) // Если ячейка правильная, ставим её нормальный цвет
            ui->tableWidget->item(i, 0)->setBackground(defaultBackgroundColor);
        else
        {
            // Если нет, то меняем флаг правильности массива и ставим для ячейки красный цвет
            okArr = false;
            ui->tableWidget->item(i, 0)->setBackground(Qt::red);
        }
    }

    int sum = 0;
    if (okArr)
    {
        // Если все значения в массиве правильные, то проходим по массиву и находим сумму
        for (int i = 0; i < arrSize; i++)
            sum += arr[i];
        // Выводим сумму всех значениё / их количество
        ui->labelAver->setNum(sum / arrSize);
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelAver->setText("Error");
    }

    noAutoChange = true;

    // Очистка других полей (для красоты), хоть и функционально это не очень удобно иногда пожалуй
    ui->labelMin->clear();
    ui->labelMax->clear();
    ui->labelSum->clear();
}
