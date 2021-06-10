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

    // Очистка поля с результатом
    ui->labelResult->clear();
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

    // Очистка поля с результатом
    ui->labelResult->clear();
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
        ui->labelResult->setNum(sum);
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Ошибка
        ui->labelResult->setText("Ошибка");
    }

    noAutoChange = true;
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
        ui->labelResult->setNum(min);
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Ошибка
        ui->labelResult->setText("Ошибка");
    }

    noAutoChange = true;
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
        ui->labelResult->setNum(max);
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Ошибка
        ui->labelResult->setText("Ошибка");
    }

    noAutoChange = true;
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
        ui->labelResult->setNum(sum / arrSize);
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Ошибка
        ui->labelResult->setText("Ошибка");
    }

    noAutoChange = true;
}

// Сортировка пузырьком
void MainWindow::on_pushButtonSort1_clicked()
{
    noAutoChange = false;
    QElapsedTimer myTimer;
    myTimer.start();

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

    if (okArr)
    {
        // Сортировка
        // Перебор всех значений и установка их в нужном порядке
        for (int i = 0; i < arrSize; i++)
        {
            for(int j = 0; j < arrSize - i - 1; j++)
            {
                if(arr[j] > arr[j + 1])
                {
                    int tmp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = tmp;
                }
            }
        }

        for (int i = 0; i < arrSize; i++)
        {
            // Вставляем в ячейки таблицы значения из массива и устанавливаем цвет ячеек на стандартный
            ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
            ui->tableWidget->item(i, 0)->setBackground(defaultBackgroundColor);
        }

        int nMilliseconds = myTimer.elapsed();
        ui->labelResult->setText(QString::number(nMilliseconds) + "ms");
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Ошибка
        ui->labelResult->setText("Ошибка");
    }

    noAutoChange = true;
}

// Гномья сортировка
void MainWindow::on_pushButtonSort2_clicked()
{
    noAutoChange = false;
    QElapsedTimer myTimer;
    myTimer.start();

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

    if (okArr)
    {
        // Сортировка
        int i = 0;
        while (i < arrSize)
        {
            if (i == 0 || arr[i - 1] <= arr[i])
            {
                // i увеличивается пока не встретится с первым неотсорированным элементом
                i++;
            }
            else
            {
                // Если встречаемся с неотсортированным элементом, мы их меняем местами
                // и уменьшаем i, таким образом "передвигая" элемент до места, где он будет больше предыдущего
                int tmp = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = tmp;
                i -= 1;
            }
        }

        for (int i = 0; i < arrSize; i++)
        {
            // Вставляем в ячейки таблицы значения из массива и устанавливаем цвет ячеек на стандартный
            ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
            ui->tableWidget->item(i, 0)->setBackground(defaultBackgroundColor);
        }

        int nMilliseconds = myTimer.elapsed();
        ui->labelResult->setText(QString::number(nMilliseconds) + "ms");
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Ошибка
        ui->labelResult->setText("Ошибка");
    }

    noAutoChange = true;
}

// Быстрая сортировка значительно сложнее других, так что тут не обойтись без разделения на отдельные методы

// Метод для разделения массива и, собственно, сортировки
int partition (int arr[], int low, int high)
{
    int pivot = arr[high]; // Опорное значение
    int i = (low - 1); // Индекс меньшего элемента

    for (int j = low; j <= high- 1; j++)
    {
        // Если текущий элемент меньше или равен опорному значению
        if (arr[j] <= pivot)
        {
            i++; // Увеличение индекса меньшего элемента
            int tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    int tmp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = tmp;
    return (i + 1);
}

// Метод для рекурсивной сортировки массива по частям
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        // Индекс разделения. Элемент с этим индексом на своём месте
        int pi = partition(arr, low, high);

        // Раздельно сортируем элементы до и после разделения
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// Быстрая сортировка
void MainWindow::on_pushButtonSort3_clicked()
{
    noAutoChange = false;
    QElapsedTimer myTimer;
    myTimer.start();

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

    if (okArr)
    {
        // Сортировка
        quickSort(arr, 0, arrSize - 1);

        for (int i = 0; i < arrSize; i++)
        {
            // Вставляем в ячейки таблицы значения из массива и устанавливаем цвет ячеек на стандартный
            ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
            ui->tableWidget->item(i, 0)->setBackground(defaultBackgroundColor);
        }

        int nMilliseconds = myTimer.elapsed();
        ui->labelResult->setText(QString::number(nMilliseconds) + "ms");
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Ошибка
        ui->labelResult->setText("Ошибка");
    }

    noAutoChange = true;
}

// Сортировка расчёской
void MainWindow::on_pushButtonSort4_clicked()
{
    noAutoChange = false;
    QElapsedTimer myTimer;
    myTimer.start();

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

    if (okArr)
    {
        // Изначально берутся максимально отдалённые элементы
        int step = arrSize;
        bool przn = true;
        // Цикл работает, пока массив не будет отсортирован
        while (step > 1 || przn)
        {
            // Пока не будут сравниваться соседние элементы, "круг" сравнения сужается
            // на определённое число. Число 1.247 является средним оптимальным
            if (step > 1)
            {
                step /= 1.247;
            }
            przn = false;
            int i = 0;
            while (i + step < arrSize)
            {
                if (arr[i] > arr[i + step])
                {
                    // Если предыдущий элемент больше последующего, меняем их местами
                    int tmp = arr[i];
                    arr[i] = arr[i + step];
                    arr[i + step] = tmp;
                    przn = true;
                }
                i++;
            }
        }

        for (int i = 0; i < arrSize; i++)
        {
            // Вставляем в ячейки таблицы значения из массива и устанавливаем цвет ячеек на стандартный
            ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
            ui->tableWidget->item(i, 0)->setBackground(defaultBackgroundColor);
        }

        int nMilliseconds = myTimer.elapsed();
        ui->labelResult->setText(QString::number(nMilliseconds) + "ms");
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Ошибка
        ui->labelResult->setText("Ошибка");
    }

    noAutoChange = true;
}

// Обезьянья сортировка
void MainWindow::on_pushButtonSort5_clicked()
{
    noAutoChange = false;
    QElapsedTimer myTimer;
    myTimer.start();

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

    if (okArr)
    {
        // Сортировка
        bool sorted = false;
        // Цикл работает, пока массив не отсортирован
        while (!sorted)
        {
            int size = arrSize;
            // Простая проверка на отсортированность перебором с конца
            while (--size > 0)
            {
                if (arr[size - 1] > arr[size])
                {
                    sorted = false;
                    break;
                }
                sorted = true;
            }
            // Если после проверки массив оказалось, что массив всё ещё не отсортирован
            if (!sorted)
            {
                for (int i = 0; i < arrSize; i++)
                {
                    // Элементы перемешиваются друг с другом в случайные места
                    int tmp = arr[i];
                    int rnd = rand() % arrSize;
                    arr[i] = arr[rnd];
                    arr[rnd] = tmp;
                }
            }
        }

        for (int i = 0; i < arrSize; i++)
        {
            // Вставляем в ячейки таблицы значения из массива и устанавливаем цвет ячеек на стандартный
            ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
            ui->tableWidget->item(i, 0)->setBackground(defaultBackgroundColor);
        }

        int nMilliseconds = myTimer.elapsed();
        ui->labelResult->setText(QString::number(nMilliseconds) + "ms");
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Ошибка
        ui->labelResult->setText("Ошибка");
    }

    noAutoChange = true;
}
