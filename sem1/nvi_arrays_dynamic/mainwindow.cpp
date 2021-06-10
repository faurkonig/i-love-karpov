#include "mainwindow.h"
#include "ui_mainwindow.h"

// Хранение фона для хранения
// Чтобы работало красиво не только в светлой теме
static QColor defaultBackgroundColor;
static QPalette redLineEditPalette, defaultLineEditPalette;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->spinBox->setMaximum(MAX_ARR); // Установка максимального числа значений в спинбокс
    noAutoChange = true; // Переменная для "блокировки" onCellChanged в таблице
    // Нахождение "нормального" цвета для фона клеток таблицы
    defaultBackgroundColor = ui->tableWidget->palette().color(QPalette::Base);

    redLineEditPalette.setColor(QPalette::Base, Qt::red);
    defaultLineEditPalette.setColor(QPalette::Base, defaultBackgroundColor);
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

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

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

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

    noAutoChange = true;

    ui->labelResult->clear();
}

void MainWindow::on_pushButtonSum_clicked()
{
    noAutoChange = false;

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

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
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelResult->setText("Ошибка");
    }

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

    noAutoChange = true;

}

void MainWindow::on_pushButtonMin_clicked()
{
    noAutoChange = false;

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

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
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelResult->setText("Ошибка");
    }

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

    noAutoChange = true;
}

void MainWindow::on_pushButtonMax_clicked()
{
    noAutoChange = false;

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

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
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelResult->setText("Ошибка");
    }

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

    noAutoChange = true;
}

void MainWindow::on_pushButtonAverage_clicked()
{
    noAutoChange = false;

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

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
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelResult->setText("Ошибка");
    }

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

    noAutoChange = true;
}

// Сортировка пузырьком
void MainWindow::on_pushButtonSort1_clicked()
{
    noAutoChange = false;
    // Таймер для замера затраченного на весь процесс времени
    QElapsedTimer myTimer;
    myTimer.start();

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

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
            // Вставляем в ячейки таблицы значения из массива
            ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
        }

        // Вывод времени в поле ответа
        int nMilliseconds = myTimer.elapsed();
        ui->labelResult->setText(QString::number(nMilliseconds) + "мс");
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelResult->setText("Ошибка");
    }

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

    noAutoChange = true;
}

// Гномья сортировка
void MainWindow::on_pushButtonSort2_clicked()
{
    noAutoChange = false;
    // Таймер для замера затраченного на весь процесс времени
    QElapsedTimer myTimer;
    myTimer.start();

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

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
            // Вставляем в ячейки таблицы значения из массива
            ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
        }

        // Вывод времени в поле ответа
        int nMilliseconds = myTimer.elapsed();
        ui->labelResult->setText(QString::number(nMilliseconds) + "мс");
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelResult->setText("Ошибка");
    }

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

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
    // Таймер для замера затраченного на весь процесс времени
    QElapsedTimer myTimer;
    myTimer.start();

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

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
            // Вставляем в ячейки таблицы значения из массива
            ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
        }

        // Вывод времени в поле ответа
        int nMilliseconds = myTimer.elapsed();
        ui->labelResult->setText(QString::number(nMilliseconds) + "мс");
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelResult->setText("Ошибка");
    }

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

    noAutoChange = true;
}

// Сортировка расчёской
void MainWindow::on_pushButtonSort4_clicked()
{
    noAutoChange = false;
    // Таймер для замера затраченного на весь процесс времени
    QElapsedTimer myTimer;
    myTimer.start();

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

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
            // Вставляем в ячейки таблицы значения из массива
            ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
        }

        // Вывод времени в поле ответа
        int nMilliseconds = myTimer.elapsed();
        ui->labelResult->setText(QString::number(nMilliseconds) + "мс");
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelResult->setText("Ошибка");
    }

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

    noAutoChange = true;
}

// Обезьянья сортировка
void MainWindow::on_pushButtonSort5_clicked()
{
    noAutoChange = false;
    // Таймер для замера затраченного на весь процесс времени
    QElapsedTimer myTimer;
    myTimer.start();

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

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
            // Вставляем в ячейки таблицы значения из массива
            ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
        }

        // Вывод времени в поле ответа
        int nMilliseconds = myTimer.elapsed();
        ui->labelResult->setText(QString::number(nMilliseconds) + "мс");
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelResult->setText("Ошибка");
    }

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

    noAutoChange = true;
}

void MainWindow::on_pushButtonCopies_clicked()
{
    noAutoChange = false;
    // Таймер для замера затраченного на весь процесс времени
    QElapsedTimer myTimer;
    myTimer.start();

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

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

        // Удаление дубликатов
        for (int i = 1; i < arrSize; i++)
        {
            // Если текущий элемент равен предыдущему
            if (arr[i] == arr[i - 1])
            {
                // Уменьшается количество элементов
                arrSize--;
                // Сдвиг оставшихся элементов в массиве
                for (int j = i; j < arrSize; j++)
                {
                    arr[j] = arr[j + 1];
                }
                i--; // i уменьшается на 1 для того, чтобы не "перескачить" через некоторые повторяющиеся элементы
            }
        }

        // Изменение кол-ва значений в таблице на значение из спинбокса
        ui->tableWidget->setRowCount(arrSize);
        ui->spinBox->setValue(arrSize);

        for (int i = 0; i < arrSize; i++)
        {
            // Вставляем в ячейки таблицы значения из массива
            ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
        }

        // Вывод времени в поле ответа
        int nMilliseconds = myTimer.elapsed();
        ui->labelResult->setText(QString::number(nMilliseconds) + "мс");
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelResult->setText("Ошибка");
    }

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

    noAutoChange = true;
}

void MainWindow::on_pushButtonSearch1_clicked()
{
    noAutoChange = false;

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

    // Флаги для проверки ячеек на корректность значений
    bool okArr = true, okCell = true, okField = true;

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

    int value = ui->lineEditSearch->text().toInt(&okField);

    if (okArr)
    {
        if (okField)
        {
            // Поиск
            int index = -1;

            // Записывает первый совпадающий индекс и выходит из цикла
            for (int i = 0; i < arrSize; i++)
            {
                if (arr[i] == value)
                {
                    index = i;
                    break;
                }
            }

            // Если элемент найден (index не равен -1), он выводится
            if (index > -1)
                ui->labelResult->setNum(index);
            else
                ui->labelResult->setText("Не найдено");

            ui->lineEditSearch->setPalette(defaultLineEditPalette);
        }
        else
        {
            ui->lineEditSearch->setPalette(redLineEditPalette);
            ui->labelResult->setText("Ошибка");
        }
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelResult->setText("Ошибка");
    }

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

    noAutoChange = true;
}

// Рекурсивная функция бинарного поиска
int binarySearch(int arr[], int start, int end, int value) {
   if (start <= end) {
       // Выбирается середина, если серединное значени равно искомому, то она выводится
      int middle = (start + end) / 2;
      if (arr[middle] == value)
        return middle;

      // Если серединнок значение больше искомого, то индекс ищется в первой половине, иначе во второй
      if (arr[middle] > value)
        return binarySearch(arr, start, middle - 1, value);

      return binarySearch(arr, middle + 1, end, value);
   }
   return -1;
}

void MainWindow::on_pushButtonSearch2_clicked()
{
    noAutoChange = false;

    int *arr = nullptr; // Создаём указатель динамической памяти
    int arrSize = ui->spinBox->value();
    arr = new int[arrSize]; // Выделяем память на нужное нам кол-во ячеек

    // Флаги для проверки ячеек на корректность значений
    bool okArr = true, okCell = true, okField = true;

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

    int value = ui->lineEditSearch->text().toInt(&okField);

    if (okArr)
    {
        if (okField)
        {
            // Сортировка
            quickSort(arr, 0, arrSize - 1);

            // Поиск
            int index = binarySearch(arr, 0 , arrSize - 1, value);

            // Если элемент найден (index не равен -1), он выводится
            if (index > -1)
                ui->labelResult->setNum(index);
            else
                ui->labelResult->setText("Не найдено");

            for (int i = 0; i < arrSize; i++)
            {
                // Вставляем в ячейки таблицы значения из массива
                ui->tableWidget->item(i, 0)->setText(QString::number(arr[i]));
            }

            ui->lineEditSearch->setPalette(defaultLineEditPalette);
        }
        else
        {
            ui->lineEditSearch->setPalette(redLineEditPalette);
            ui->labelResult->setText("Ошибка");
        }
    }
    else
    {
        // Если же какие-то значения не правильные, то выводим надпись Error
        ui->labelResult->setText("Ошибка");
    }

    delete [] arr; // Освобождаем память
    arr = nullptr; // Отмечаем, что память свободна

    noAutoChange = true;
}
