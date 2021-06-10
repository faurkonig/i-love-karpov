#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Конструктор
    ui->setupUi(this);

    ui->tableWidget->setColumnCount(COL_COUNT);

    defaultColor = ui->tableWidget->palette().color(QPalette::Base);
    errorColor = QColor(229, 43, 80);
}

MainWindow::~MainWindow()
{
    // Деструктор
    delete ui;
}


void MainWindow::on_spinBox_valueChanged(int newCount)
{
    // Обработчик выбора количества элементов
    int curCount = primitives.count();
    int deltaCount = abs(newCount - curCount);

    ui->tableWidget->setRowCount(newCount * 2);

    if (newCount > curCount)
    {
        for (int i = 0; i < deltaCount; i++)
        {
            // Создание класса со значениями
            primitives.append(new Pixel());
            incorrects.append({false, false});

            // Создание заголовка для поля с комбобоксом
            int index = (curCount + i) * 2;
            QTableWidgetItem * twi = new QTableWidgetItem;
            twi->setText("Тип объекта");
            twi->setFlags(twi->flags() & 0xffd);
            ui->tableWidget->setItem(index, 0, twi);
            // Создание комбобокса
            QComboBox * cb = new QComboBox;
            cb->addItem("Пиксель");
            cb->addItem("Линия");
            cb->addItem("Прямоугольник");
            cb->addItem("Эллипс");
            cb->addItem("Дуга");
            cb->addItem("Ломаная");
            cb->addItem("Многоугольник");
            // Установка всех нужных значений и вставление комбобокса
            cb->setProperty("index", curCount + i);
            connect(cb, SIGNAL(activated(int)), this, SLOT(typeComboBox_clicked(int)));
            ui->tableWidget->removeCellWidget(index + 1, 0);
            ui->tableWidget->setCellWidget(index + 1, 0, cb);
            // Генерирование всех остальных значений в таблице
            isAutoChange = true;
            genItemsPixel(index);
            isAutoChange = false;
        }
    }
    else if (newCount < curCount)
    {
        for (int i = 0; i < deltaCount; i++)
        {
            primitives.removeLast();
            incorrects.removeLast();
        }

        // Рисуем всё
        drawAll();
    }
}

void MainWindow::typeComboBox_clicked(int arg1)
{
    QComboBox * cbox = qobject_cast<QComboBox *>(sender());

    if (cbox != nullptr)
    {
        int index = cbox->property("index").toInt();
        if (arg1 != primitives[index]->type)
        {
            isAutoChange = true;

            int row = index * 2;
            clearItems(row);

            switch (arg1) {
            // пиксель
            case 0: {
                primitives[index] = new Pixel();
                incorrects[index] = {false, false};
                genItemsPixel(row);
                break;
            }
            // линия
            case 1: {
                primitives[index] = new Line();
                incorrects[index] = {false, false, false, false};
                genItemsLine(row);
                break;
            }
            // прямоугольник
            case 2: {
                primitives[index] = new Rectangle();
                incorrects[index] = {false, false, false, false};
                genItemsRectangle(row);
                break;
            }
            // эллипс
            case 3: {
                primitives[index] = new Ellipse();
                incorrects[index] = {false, false, false, false};
                genItemsEllipse(row);
                break;
            }
            // дуга
            case 4: {
                primitives[index] = new Arc();
                incorrects[index] = {false, false, false, false, false, false};
                genItemsArc(row);
                break;
            }
            // ломаная
            case 5: {
                primitives[index] = new Curve();
                incorrects[index] = {};
                genItemsCurve(row);
                break;
            }
            // многоугольник
            case 6: {
                primitives[index] = new Polygon();
                incorrects[index] = {};
                genItemsPolygon(row);
                break;
            }
            }

            isAutoChange = false;

            // Рисуем всё
            drawAll();
        }
    }
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    // Обработчик изменения ячеек таблицы
    if (!isAutoChange)
    {
        if (column > 0)
        {
            // Если ячейка изменена вручную и если это изменяемая ячейка
            QTableWidgetItem * twi = ui->tableWidget->item(row, column);
            int index = (row - (row % 2)) / 2;
            bool ok;
            int value = twi->text().toInt(&ok);

            if (ok && ((column >= 1 && column <= 4 && value >= 0) ||
                       (column >= 5 && column <= 6 && value >= -360 && value <= 360)))
            {
                // Если ячейка правильная
                twi->setBackground(defaultColor);
                incorrects[index][column - 1] = false;

                Primitive * prim = primitives[index];
                switch (prim->type)
                {
                case PrimitiveTypes::PixelT: {
                    Pixel * obj = static_cast<Pixel *>(prim);
                    if (column == 1) obj->setX(value);
                    else if (column == 2) obj->setY(value);
                    break;
                }
                case PrimitiveTypes::LineT: {
                    Line * obj = static_cast<Line *>(prim);
                    if (column == 1) obj->p1.setX(value);
                    else if (column == 2) obj->p1.setY(value);
                    if (column == 3) obj->p2.setX(value);
                    else if (column == 4) obj->p2.setY(value);
                    break;
                }
                case PrimitiveTypes::RectangleT: {
                    Rectangle * obj = static_cast<Rectangle *>(prim);
                    if (column == 1) obj->p1.setX(value);
                    else if (column == 2) obj->p1.setY(value);
                    if (column == 3) obj->p2.setX(value);
                    else if (column == 4) obj->p2.setY(value);
                    break;
                }
                case PrimitiveTypes::EllipseT: {
                    Ellipse * obj = static_cast<Ellipse *>(prim);
                    if (column == 1) obj->setX(value);
                    else if (column == 2) obj->setY(value);
                    if (column == 3) obj->setRadius1(value);
                    else if (column == 4) obj->setRadius2(value);
                    break;
                }
                case PrimitiveTypes::ArcT: {
                    Arc * obj = static_cast<Arc *>(prim);
                    if (column == 1) obj->setX(value);
                    else if (column == 2) obj->setY(value);
                    if (column == 3) obj->setRadius1(value);
                    else if (column == 4) obj->setRadius2(value);
                    if (column == 5) obj->setStartAngle(value);
                    else if (column == 6) obj->setSpanAngle(value);
                    break;
                }
                // Бесполезный default, срабатывает, когда изменяется ячейка в ломаных или многоугольниках,
                // но таких случаев нет, поэтому добавлен, чтобы не ругалось IDE
                default: {}
                }

                // Рисуем всё
                drawAll();
            }
            else
            {
                // Если значение ячейки неверно
                twi->setBackground(errorColor);
                incorrects[index][column - 1] = true;
            }
        }
    }
}


//------------------------
//- Обработчики диалогов -
//------------------------

void MainWindow::penStyleButton_clicked()
{
    // Обработчик диалога с настройкой линий
    QPushButton * cbox = qobject_cast<QPushButton *>(sender());

    if (cbox != nullptr)
    {
        int index = cbox->property("index").toInt();

        QString instTitle;
        bool isPixel = false;
        switch (primitives[index]->type)
        {
        case PrimitiveTypes::PixelT:
            instTitle = "Пиксель";
            isPixel = true;
            break;
        case PrimitiveTypes::LineT:
            instTitle = "Линия";
            break;
        case PrimitiveTypes::RectangleT:
            instTitle = "Прямоугольник";
            break;
        case PrimitiveTypes::EllipseT:
            instTitle = "Эллипс";
            break;
        case PrimitiveTypes::ArcT:
            instTitle = "Дуга";
            break;
        case PrimitiveTypes::CurveT:
            instTitle = "Ломаная";
            break;
        case PrimitiveTypes::PolygonT:
            instTitle = "Многоугольник";
            break;
        }

        PenDialog dialog;
        dialog.openDialog(instTitle, primitives[index]->pen, isPixel);

        // Рисуем всё
        drawAll();
    }
}

void MainWindow::brushStyleButton_clicked()
{
    // Обработчик диалога с настройкой заливки
    QPushButton * cbox = qobject_cast<QPushButton *>(sender());

    if (cbox != nullptr)
    {
        int index = cbox->property("index").toInt();

        QString instTitle;
        switch (primitives[index]->type)
        {
        case PrimitiveTypes::RectangleT:
            instTitle = "Прямоугольник";
            break;
        case PrimitiveTypes::EllipseT:
            instTitle = "Эллипс";
            break;
        case PrimitiveTypes::PolygonT:
            instTitle = "Многоугольник";
            break;
        // Бесполезный default просто для того, чтобы IDE не ругалось
        default: {}
        }

        BrushDialog dialog;
        dialog.openDialog(instTitle, primitives[index]->brush);

        // Рисуем всё
        drawAll();
    }
}

void MainWindow::pointsDialogButton_clicked()
{
    // Обработчик диалога с настройкой точек
    QPushButton * cbox = qobject_cast<QPushButton *>(sender());

    bool isCurve = false;
    if (cbox != nullptr)
    {
        int index = cbox->property("index").toInt();

        // Получение класса с точками и правильного название инструмента
        QString instTitle;
        switch (primitives[index]->type)
        {
        case PrimitiveTypes::CurveT:
            instTitle = "Ломаная";
            isCurve = true;
            break;
        case PrimitiveTypes::PolygonT:
            instTitle = "Многоугольник";
            break;
        // Бесполезный default добавлен просто для того, чтобы IDE не ругалось
        default: {}
        }

        // Вызов диалога
        PointsDialog dialog(this, defaultColor, errorColor);
        dialog.openDialog(instTitle, primitives[index]->points, isCurve);

        // Рисуем всё
        drawAll();
    }
}

void MainWindow::drawAll()
{
    // Функция для отрисовки всех элементов за раз
    QImage * im = &ui->widget->image;
    (*im).fill(0);
    foreach (Primitive * prim, primitives)
    {
        prim->draw(im);
    }
    ui->widget->repaint();
}


//--------------------//
//- Генераторы ячеек -//
//--------------------//
// Сделал эти функции для удобного и красивого создания ячеек

void MainWindow::clearItems(int row)
{
    // Функция для очистки всех ячеек таблицы перед установкой значений в неё
    for (int i = 1; i < COL_COUNT; i++)
    {
        QTableWidgetItem * twi = ui->tableWidget->item(row, i);
        if (twi != nullptr)
        {
            twi->setText("");
            twi->setBackground(defaultColor);
        }
        twi = ui->tableWidget->item(row + 1, i);
        if (twi != nullptr)
        {
            twi->setText("");
            twi->setBackground(defaultColor);
        }
        ui->tableWidget->removeCellWidget(row, i);
        ui->tableWidget->removeCellWidget(row + 1, i);
    }
}

void MainWindow::genItemsPixel(int row)
{
    // Функция для генерации ячеек под пиксель
    QTableWidgetItem * twi = new QTableWidgetItem;
    twi->setText("X");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 1, twi);

    twi = new QTableWidgetItem;
    twi->setText("Y");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 2, twi);

    twi = new QTableWidgetItem;
    twi->setText("Стиль точки");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 3, twi);

    for (int j = 4; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("---");
        twi->setFlags(twi->flags() & 0x1d);
        ui->tableWidget->setItem(row, j, twi);
    }

    row++;

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 1, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 2, twi);

    QPushButton * pb = new QPushButton;
    pb->setText("Задать");
    pb->setProperty("index", (row - 1) / 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(penStyleButton_clicked()));
    ui->tableWidget->setCellWidget(row, 3, pb);


    for (int j = 4; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("");
        twi->setFlags(twi->flags() & BLOCKED_FL);
        ui->tableWidget->setItem(row, j, twi);
    }
}

void MainWindow::genItemsLine(int row)
{
    // Функция для генерации ячеек под линиию
    QTableWidgetItem * twi = new QTableWidgetItem;
    twi->setText("X1");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 1, twi);

    twi = new QTableWidgetItem;
    twi->setText("Y1");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 2, twi);

    twi = new QTableWidgetItem;
    twi->setText("X2");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 3, twi);

    twi = new QTableWidgetItem;
    twi->setText("Y2");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 4, twi);

    twi = new QTableWidgetItem;
    twi->setText("Стиль линии");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 5, twi);

    for (int j = 6; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("---");
        twi->setFlags(twi->flags() & BLOCKED_FL);
        ui->tableWidget->setItem(row, j, twi);
    }

    row++;

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->removeCellWidget(row, 1);
    ui->tableWidget->setItem(row, 1, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->removeCellWidget(row, 2);
    ui->tableWidget->setItem(row, 2, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->removeCellWidget(row, 3);
    ui->tableWidget->setItem(row, 3, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->removeCellWidget(row, 4);
    ui->tableWidget->setItem(row, 4, twi);


    QPushButton * pb = new QPushButton;
    pb->setText("Задать");
    pb->setProperty("index", (row - 1) / 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(penStyleButton_clicked()));
    ui->tableWidget->setCellWidget(row, 5, pb);

    for (int j = 6; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("");
        twi->setFlags(twi->flags() & BLOCKED_FL);
        ui->tableWidget->setItem(row, j, twi);
    }
}

void MainWindow::genItemsRectangle(int row)
{
    // Функция для генерации ячеек под прямоугольник
    QTableWidgetItem * twi = new QTableWidgetItem;
    twi->setText("X1");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 1, twi);

    twi = new QTableWidgetItem;
    twi->setText("Y1");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 2, twi);

    twi = new QTableWidgetItem;
    twi->setText("X2");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 3, twi);

    twi = new QTableWidgetItem;
    twi->setText("Y2");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 4, twi);

    twi = new QTableWidgetItem;
    twi->setText("Стиль границы");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 5, twi);

    twi = new QTableWidgetItem;
    twi->setText("Стиль заливки");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 6, twi);

    for (int j = 7; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("---");
        twi->setFlags(twi->flags() & BLOCKED_FL);
        ui->tableWidget->setItem(row, j, twi);
    }

    row++;

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 1, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 2, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 3, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 4, twi);

    QPushButton * pb = new QPushButton;
    pb->setText("Задать");
    pb->setProperty("index", (row - 1) / 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(penStyleButton_clicked()));
    ui->tableWidget->setCellWidget(row, 5, pb);

    pb = new QPushButton;
    pb->setText("Задать");
    pb->setProperty("index", (row - 1) / 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(brushStyleButton_clicked()));
    ui->tableWidget->setCellWidget(row, 6, pb);

    for (int j = 7; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("");
        twi->setFlags(twi->flags() & BLOCKED_FL);
        ui->tableWidget->setItem(row, j, twi);
    }
}

void MainWindow::genItemsEllipse(int row)
{
    // Функция для генерации ячеек под эллипс
    QTableWidgetItem * twi = new QTableWidgetItem;
    twi->setText("X");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 1, twi);

    twi = new QTableWidgetItem;
    twi->setText("Y");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 2, twi);

    twi = new QTableWidgetItem;
    twi->setText("R1");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 3, twi);

    twi = new QTableWidgetItem;
    twi->setText("R2");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 4, twi);

    twi = new QTableWidgetItem;
    twi->setText("Стиль границы");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 5, twi);

    twi = new QTableWidgetItem;
    twi->setText("Стиль заливки");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 6, twi);

    for (int j = 7; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("---");
        twi->setFlags(twi->flags() & 0xfdd);
        ui->tableWidget->setItem(row, j, twi);
    }

    row++;

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 1, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 2, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 3, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 4, twi);

    QPushButton * pb = new QPushButton;
    pb->setText("Задать");
    pb->setProperty("index", (row - 1) / 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(penStyleButton_clicked()));
    ui->tableWidget->setCellWidget(row, 5, pb);

    pb = new QPushButton;
    pb->setText("Задать");
    pb->setProperty("index", (row - 1) / 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(brushStyleButton_clicked()));
    ui->tableWidget->setCellWidget(row, 6, pb);

    for (int j = 7; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("");
        twi->setFlags(twi->flags() & BLOCKED_FL);
        ui->tableWidget->setItem(row, j, twi);
    }
}

void MainWindow::genItemsArc(int row)
{
    // Функция для генерации ячеек под дугу
    QTableWidgetItem * twi = new QTableWidgetItem;
    twi->setText("X");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 1, twi);

    twi = new QTableWidgetItem;
    twi->setText("Y");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 2, twi);

    twi = new QTableWidgetItem;
    twi->setText("R1");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 3, twi);

    twi = new QTableWidgetItem;
    twi->setText("R2");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 4, twi);

    twi = new QTableWidgetItem;
    twi->setText("St");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 5, twi);

    twi = new QTableWidgetItem;
    twi->setText("Sp");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 6, twi);

    twi = new QTableWidgetItem;
    twi->setText("Стиль дуги");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 7, twi);

    for (int j = 8; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("---");
        twi->setFlags(twi->flags() & BLOCKED_FL);
        ui->tableWidget->setItem(row, j, twi);
    }

    row++;

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 1, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 2, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 3, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 4, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 5, twi);

    twi = new QTableWidgetItem;
    twi->setText("0");
    ui->tableWidget->setItem(row, 6, twi);


    QPushButton * pb = new QPushButton;
    pb->setText("Задать");
    pb->setProperty("index", (row - 1) / 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(penStyleButton_clicked()));
    ui->tableWidget->setCellWidget(row, 7, pb);

    for (int j = 8; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("");
        twi->setFlags(twi->flags() & BLOCKED_FL);
        ui->tableWidget->setItem(row, j, twi);
    }
}

void MainWindow::genItemsCurve(int row)
{
    // Функция для генерации ячеек под ломаную
    QTableWidgetItem * twi = new QTableWidgetItem;
    twi->setText("Точки ломаной");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 1, twi);

    twi = new QTableWidgetItem;
    twi->setText("Стиль линии");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 2, twi);

    for (int j = 3; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("---");
        twi->setFlags(twi->flags() & BLOCKED_FL);
        ui->tableWidget->setItem(row, j, twi);
    }

    row++;

    QPushButton * pb = new QPushButton;
    pb->setText("Задать");
    pb->setProperty("index", (row - 1) / 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(pointsDialogButton_clicked()));
    ui->tableWidget->setCellWidget(row, 1, pb);

    pb = new QPushButton;
    pb->setText("Задать");
    pb->setProperty("index", (row - 1) / 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(penStyleButton_clicked()));
    ui->tableWidget->setCellWidget(row, 2, pb);

    for (int j = 3; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("");
        twi->setFlags(twi->flags() & BLOCKED_FL);
        ui->tableWidget->setItem(row, j, twi);
    }
}

void MainWindow::genItemsPolygon(int row)
{
    // Функция для генерации ячеек под многоугольник
    QTableWidgetItem * twi = new QTableWidgetItem;
    twi->setText("Точки многоугольника");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 1, twi);

    twi = new QTableWidgetItem;
    twi->setText("Стиль линии");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 2, twi);

    twi = new QTableWidgetItem;
    twi->setText("Стиль заливки");
    twi->setFlags(twi->flags() & HEADER_FL);
    ui->tableWidget->setItem(row, 3, twi);

    for (int j = 4; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("---");
        twi->setFlags(twi->flags() & BLOCKED_FL);
        ui->tableWidget->setItem(row, j, twi);
    }

    row++;

    QPushButton * pb = new QPushButton;
    pb->setText("Задать");
    pb->setProperty("index", (row - 1) / 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(pointsDialogButton_clicked()));
    ui->tableWidget->setCellWidget(row, 1, pb);

    pb = new QPushButton;
    pb->setText("Задать");
    pb->setProperty("index", (row - 1) / 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(penStyleButton_clicked()));
    ui->tableWidget->setCellWidget(row, 2, pb);

    pb = new QPushButton;
    pb->setText("Задать");
    pb->setProperty("index", (row - 1) / 2);
    connect(pb, SIGNAL(clicked()), this, SLOT(brushStyleButton_clicked()));
    ui->tableWidget->setCellWidget(row, 3, pb);

    for (int j = 4; j < COL_COUNT; j++)
    {
        twi = new QTableWidgetItem;
        twi->setText("");
        twi->setFlags(twi->flags() & BLOCKED_FL);
        ui->tableWidget->setItem(row, j, twi);
    }
}

void MainWindow::genItemsFromClasses(QVector<Primitive *> * prims)
{
    // Функция для вывода всех значений из классов примитивов в таблицу
    // Нужна для загрузки файлаs
    int count = prims->count();
    ui->spinBox->setValue(count);
    for (int i = 0; i < count; i++)
    {
        int row = (i * 2) + 1;
        primitives = *prims;
        Primitive * curPrim = primitives[i];

        // Устанавливаем тип в комбобоксе
        PrimitiveTypes curType = curPrim->type;
        QComboBox * cb = qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(row, 0));
        if (cb != nullptr)
            cb->setCurrentIndex(static_cast<int>(curPrim->type));

        isAutoChange = true;
        clearItems(row - 1);

        switch (curType) {
        // пиксель
        case PrimitiveTypes::PixelT: {
            incorrects.append(QVector<bool>{false, false});
            genItemsPixel(row - 1);
            Pixel * obj = static_cast<Pixel *>(primitives[i]);
            ui->tableWidget->item(row, 1)->setText(QString::number(obj->getX()));
            ui->tableWidget->item(row, 2)->setText(QString::number(obj->getY()));
            break;
        }
        // линия
        case PrimitiveTypes::LineT: {
            incorrects.append(QVector<bool>{false, false, false, false});
            genItemsLine(row - 1);
            Line * obj = static_cast<Line *>(primitives[i]);
            ui->tableWidget->item(row, 1)->setText(QString::number(obj->p1.getX()));
            ui->tableWidget->item(row, 2)->setText(QString::number(obj->p1.getY()));
            ui->tableWidget->item(row, 3)->setText(QString::number(obj->p2.getX()));
            ui->tableWidget->item(row, 4)->setText(QString::number(obj->p2.getY()));
            break;
        }
        // прямоугольник
        case PrimitiveTypes::RectangleT: {
            incorrects.append(QVector<bool>{false, false, false, false});
            genItemsRectangle(row - 1);
            Rectangle * obj = static_cast<Rectangle *>(primitives[i]);
            ui->tableWidget->item(row, 1)->setText(QString::number(obj->p1.getX()));
            ui->tableWidget->item(row, 2)->setText(QString::number(obj->p1.getY()));
            ui->tableWidget->item(row, 3)->setText(QString::number(obj->p2.getX()));
            ui->tableWidget->item(row, 4)->setText(QString::number(obj->p2.getY()));
            break;
        }
        // эллипс
        case PrimitiveTypes::EllipseT: {
            incorrects.append(QVector<bool>{false, false, false, false});
            genItemsEllipse(row - 1);
            Ellipse * obj = static_cast<Ellipse *>(primitives[i]);
            ui->tableWidget->item(row, 1)->setText(QString::number(obj->getX()));
            ui->tableWidget->item(row, 2)->setText(QString::number(obj->getY()));
            ui->tableWidget->item(row, 3)->setText(QString::number(obj->getRadius1()));
            ui->tableWidget->item(row, 4)->setText(QString::number(obj->getRadius2()));
            break;
        }
        // дуга
        case PrimitiveTypes::ArcT: {
            incorrects.append(QVector<bool>{false, false, false, false, false, false});
            genItemsArc(row - 1);
            Arc * obj = static_cast<Arc *>(primitives[i]);
            ui->tableWidget->item(row, 1)->setText(QString::number(obj->getX()));
            ui->tableWidget->item(row, 2)->setText(QString::number(obj->getY()));
            ui->tableWidget->item(row, 3)->setText(QString::number(obj->getRadius1()));
            ui->tableWidget->item(row, 4)->setText(QString::number(obj->getRadius2()));
            ui->tableWidget->item(row, 5)->setText(QString::number(obj->getStartAngle()));
            ui->tableWidget->item(row, 6)->setText(QString::number(obj->getSpanAngle()));
            break;
        }
        // ломаная
        case PrimitiveTypes::CurveT: {
            incorrects.append(QVector<bool>{});
            genItemsCurve(row - 1);
            break;
        }
        // многоугольник
        case PrimitiveTypes::PolygonT: {
            incorrects.append(QVector<bool>{});
            genItemsPolygon(row - 1);
            break;
        }
        }

        isAutoChange = false;

        // Всё рисуем
        drawAll();
    }
}


//--------------------------------
//- Обработчики работы с файлами -
//--------------------------------


void MainWindow::on_action_JSON_save_triggered()
{
    // Сохранение таблицы в JSON
    // Проверка правильности значений
    bool okArr = true;
    foreach (QVector<bool> row, incorrects)
    {
        foreach (bool cell, row)
        {
            if (cell)
            {
                okArr = false;
                break;
            }
        }
        if (!okArr) break;
    }

    if (!okArr)
    {
        // Если есть неверные значения
        QMessageBox msgBox;
        msgBox.setWindowTitle("Предупреждение");
        msgBox.setText("В таблице указаны неверные значения. Будут сохранены последние корректные значения.\nПри нажатии \"Отмена\", сохранение будет отменено");
        msgBox.setIcon(QMessageBox::Warning);
        QAbstractButton * yesButton = msgBox.addButton("Всё равно сохранить", QMessageBox::YesRole);
        msgBox.addButton("Отмена", QMessageBox::NoRole);
        msgBox.exec();
        if (msgBox.clickedButton() == yesButton)
            okArr = true;
    }

    if (okArr)
    {
        // Если изначально значения были правильные или если пользователь согласился сохранить хоть что-нибудь
        QJsonArray rootArray;
        int primitiveCount = primitives.count();
        for (int i = 0; i < primitiveCount; i++)
        {
            // Сохранение каждого отдельного примитива
            Primitive * prim = primitives[i];
            QJsonObject primJsonObject;
            primJsonObject["type"] = static_cast<int>(prim->type);

            if (prim->pen != nullptr)
            {
                // Сохранение карандаша
                QJsonObject penObj;
                // Установка цвета
                int rr, gg, bb, aa;
                prim->pen->getRGBA(rr, gg, bb, aa);
                QJsonObject colorObj;
                colorObj["red"] = rr;
                colorObj["green"] = gg;
                colorObj["blue"] = bb;
                colorObj["alpha"] = aa;
                penObj["color"] = colorObj;
                // Установка других значений
                penObj["width"] = prim->pen->getWeight();
                penObj["style"] = prim->pen->getStyle();
                primJsonObject["pen"] = penObj;
            }

            if (prim->brush != nullptr)
            {
                // Сохранение карандаша
                QJsonObject brushObj;
                // Установка цвета
                int rr, gg, bb, aa;
                prim->brush->getRGBA(rr, gg, bb, aa);
                QJsonObject colorObj;
                colorObj["red"] = rr;
                colorObj["green"] = gg;
                colorObj["blue"] = bb;
                colorObj["alpha"] = aa;
                brushObj["color"] = colorObj;
                // Установка других значений
                brushObj["style"] = prim->brush->getStyle();
                primJsonObject["brush"] = brushObj;
            }

            if (prim->points != nullptr)
            {
                // Сохранение точек
                QJsonArray pointsArr;
                QVector<Point> * points = prim->points;
                int pointsCount = points->count();
                for (int k = 0; k < pointsCount; k++)
                {
                    // Сохранение каждой отдельной точки
                    QJsonObject pointObj {
                        {"x", (*points)[k].getX()},
                        {"y", (*points)[k].getY()}
                    };
                    pointsArr.append(pointObj);
                }
                primJsonObject["points"] = pointsArr;
            }

            switch (prim->type)
            {
            // Пиксель
            case PrimitiveTypes::PixelT: {
                Pixel * obj = static_cast<Pixel *>(prim);
                primJsonObject["x"] = obj->getX();
                primJsonObject["y"] = obj->getY();
                break;
            }
            // Линия
            case PrimitiveTypes::LineT: {
                Line * obj = static_cast<Line *>(prim);
                primJsonObject["start"] = QJsonObject {
                    {"x", obj->p1.getX()},
                    {"y", obj->p1.getY()}
                };
                primJsonObject["end"] = QJsonObject {
                    {"x", obj->p2.getX()},
                    {"y", obj->p2.getY()}
                };
                break;
            }
            // Прямоугольник
            case PrimitiveTypes::RectangleT: {
                Rectangle * obj = static_cast<Rectangle *>(prim);
                primJsonObject["start"] = QJsonObject {
                {"x", obj->p1.getX()},
                {"y", obj->p1.getY()}
                };
                primJsonObject["end"] = QJsonObject {
                {"x", obj->p2.getX()},
                {"y", obj->p2.getY()}
                };
                break;
            }
            // Эллипс
            case PrimitiveTypes::EllipseT: {
                Ellipse * obj = static_cast<Ellipse *>(prim);
                primJsonObject["x"] = obj->getX();
                primJsonObject["y"] = obj->getY();
                primJsonObject["radius1"] = obj->getRadius1();
                primJsonObject["radius2"] = obj->getRadius2();
                break;
            }
            // Дуга
            case PrimitiveTypes::ArcT: {
                Arc * obj = static_cast<Arc *>(prim);
                primJsonObject["x"] = obj->getX();
                primJsonObject["y"] = obj->getY();
                primJsonObject["radius1"] = obj->getRadius1();
                primJsonObject["radius2"] = obj->getRadius2();
                primJsonObject["start_angle"] = obj->getStartAngle();
                primJsonObject["span_angle"] = obj->getSpanAngle();
                break;
            }
            // Все предыдущие случае покрывают все значения, так что этот бесполезный default
            // был добавлен для того, чтобы IDE не ругался
            default: {}
            }

            // Вставляем объект примитива в общий список
            rootArray.append(primJsonObject);
        }

        QJsonDocument jsonDocument(rootArray); // Конечный JSON объект
        // Получение пути к файлу
        QString filename = QFileDialog::getSaveFileName(this, "Сохранить файл", DEF_FILE_DIR, tr("JSON files (*.json)"));
        if (filename.isEmpty())
        {
            // Если не был выбран файл
            QMessageBox::warning(this, "Ошибка", "Файл не выбран");
        }
        else
        {
            QFile file(filename);
            if (file.open(QIODevice::WriteOnly))
            {
                // Если удалось открыть файл для записи
                QByteArray ba = jsonDocument.toJson();
                // Записываем всё в файл и закрываем
                file.write(ba);
                file.close();
            }
            else
            {
                QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
            }
        }

    }
}

void MainWindow::on_action_BIN_save_triggered()
{
    // Сохранение таблицы
    // Проверка правильности значений
    bool okArr = true;
    foreach (QVector<bool> row, incorrects)
    {
        foreach (bool cell, row)
        {
            if (cell)
            {
                okArr = false;
                break;
            }
        }
        if (!okArr) break;
    }

    if (!okArr)
    {
        // Если есть неверные значения
        QMessageBox msgBox;
        msgBox.setWindowTitle("Предупреждение");
        msgBox.setText("В таблице указаны неверные значения. Будут сохранены последние корректные значения.\nПри нажатии \"Отмена\", сохранение будет отменено");
        msgBox.setIcon(QMessageBox::Warning);
        QAbstractButton * yesButton = msgBox.addButton("Всё равно сохранить", QMessageBox::YesRole);
        msgBox.addButton("Отмена", QMessageBox::NoRole);
        msgBox.exec();
        if (msgBox.clickedButton() == yesButton)
            okArr = true;
    }

    if (okArr)
    {
        // Если изначально значения были правильные или если пользователь согласился сохранить хоть что-нибудь
        // Получение пути к файлу
        QString filename = QFileDialog::getSaveFileName(this, "Сохранить файл", DEF_FILE_DIR, tr("Binary files (*.bin)"));
        if (filename.isEmpty())
        {
            // Если не был выбран файл
            QMessageBox::warning(this, "Ошибка", "Файл не выбран");
        }
        else
        {
            QFile file(filename);
            if (file.open(QIODevice::WriteOnly))
            {
                // Если удалось открыть файл для записи
                char mas[intSz];
                // Записываем количество элементов в файл
                int count = primitives.count();
                memcpy(&mas, &count, intSz);
                file.write(mas, intSz);
                for (int i = 0; i < count; i++)
                {
                    // Сохранение 1 элемента
                    Primitive * prim = primitives[i];

                    // Сохранение типа элемента
                    int type = prim->type;
                    memcpy(&mas, &type, intSz);
                    file.write(mas, intSz);

                    // Сохранение карандаша
                    if (prim->pen != nullptr)
                    {
                        int red, green, blue, alpha, width, style;
                        prim->pen->getRGBA(red, green, blue, alpha);
                        width = prim->pen->getWeight();
                        style = prim->pen->getStyle();

                        memcpy(&mas, &red, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &green, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &blue, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &alpha, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &width, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &style, intSz);
                        file.write(mas, intSz);
                    }
                    // Сохранение заливки
                    if (prim->brush != nullptr)
                    {
                        int red, green, blue, alpha, style;
                        prim->brush->getRGBA(red, green, blue, alpha);
                        style = prim->brush->getStyle();

                        memcpy(&mas, &red, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &green, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &blue, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &alpha, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &style, intSz);
                        file.write(mas, intSz);
                    }
                    // Сохранение точек
                    if (prim->points != nullptr)
                    {
                        int pc = prim->points->count();
                        memcpy(&mas, &pc, intSz);
                        file.write(mas, intSz);
                        for (int j = 0; j < pc; j++)
                        {
                            Point point = (*prim->points)[j];
                            int px, py;
                            point.getXY(px, py);
                            memcpy(&mas, &px, intSz);
                            file.write(mas, intSz);
                            memcpy(&mas, &py, intSz);
                            file.write(mas, intSz);
                        }
                    }

                    switch (type)
                    {
                    // Пиксель
                    case PixelT: {
                        Pixel * obj = static_cast<Pixel *>(prim);
                        int x, y;
                        obj->getXY(x, y);
                        memcpy(&mas, &x, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &y, intSz);
                        file.write(mas, intSz);
                        break;
                    }
                    // Линия
                    case LineT: {
                        Line * obj = static_cast<Line *>(prim);
                        int x1, y1, x2, y2;
                        obj->p1.getXY(x1, y1);
                        obj->p2.getXY(x2, y2);
                        memcpy(&mas, &x1, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &y1, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &x2, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &y2, intSz);
                        file.write(mas, intSz);
                        break;
                    }
                    // Прямоугольник
                    case RectangleT: {
                        Rectangle * obj = static_cast<Rectangle *>(prim);
                        int x1, y1, x2, y2;
                        obj->p1.getXY(x1, y1);
                        obj->p2.getXY(x2, y2);
                        memcpy(&mas, &x1, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &y1, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &x2, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &y2, intSz);
                        file.write(mas, intSz);
                        break;
                    }
                    // Эллипс
                    case EllipseT: {
                        Ellipse * obj = static_cast<Ellipse *>(prim);
                        int x, y, r1, r2;
                        obj->getXY(x, y);
                        r1 = obj->getRadius1();
                        r2 = obj->getRadius2();
                        memcpy(&mas, &x, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &y, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &r1, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &r2, intSz);
                        file.write(mas, intSz);
                        break;
                    }
                    // Дуга
                    case ArcT: {
                        Arc * obj = static_cast<Arc *>(prim);
                        int x, y, r1, r2, a1, a2;
                        obj->getXY(x, y);
                        r1 = obj->getRadius1();
                        r2 = obj->getRadius2();
                        a1 = obj->getStartAngle();
                        a2 = obj->getSpanAngle();
                        memcpy(&mas, &x, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &y, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &r1, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &r2, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &a1, intSz);
                        file.write(mas, intSz);
                        memcpy(&mas, &a2, intSz);
                        file.write(mas, intSz);
                        break;
                    }
                    // Для ломаной и многоугольника мы уже сохранили точки, так что этот default бессмысленный,
                    // но он добавлен, чтобы IDE не ругался
                    default: {}
                    }
                }

                // Закрываем файл
                file.close();
            }
            else
            {
                QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
            }
        }

    }
}

void MainWindow::on_action_JSON_open_triggered()
{
    // Загрузка данных из JSON в таблицу
    QString filename = QFileDialog::getOpenFileName(this, "Открыть файл", DEF_FILE_DIR, tr("JSON files (*.json)"));
    if (filename.isEmpty())
    {
        // Если не был выбран файл
        QMessageBox::warning(this, "Ошибка", "Файл не выбран");
    }
    else
    {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly))
        {
            // Если удалось открыть файл для чтения
            // Всё считываем и закрываем
            QByteArray ba = file.readAll();
            file.close();
            QJsonParseError error;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &error);
            if (error.error != error.NoError)
            {
                // Если парсинг JSON не удался
                QMessageBox::warning(this, "Ошибка", "Файл содержит критические ошибки синтаксиса:\n" + error.errorString());
            }
            else
            {
                if (jsonDocument.isArray())
                {
                    QVector<Primitive *> newPrimitives;
                    QJsonArray rootArray = jsonDocument.array();
                    int elemCount = rootArray.count();
                    if (elemCount < 100)
                    {
                        int errCount = 0;
                        for (int i = 0; i < elemCount; i++)
                        {
                            if (!rootArray[i].isObject())
                            {
                                // Если элемент списка - не объект
                                errCount++;
                                continue;
                            }
                            QJsonObject curPrim = rootArray[i].toObject();

                            bool okType;
                            PrimitiveTypes type = static_cast<PrimitiveTypes>(jsonGetNum(curPrim["type"], &okType));
                            if (!okType || type < 0 || type > 6)
                            {
                                // Если тип указан неверно
                                errCount++;
                                continue;
                            }

                            Pen * curPenPtr = nullptr;
                            Brush * curBrushPtr = nullptr;
                            QVector<Point> * curPointsPtr = nullptr;

                            // Получение карандаша
                            curPenPtr = jsonGetPen(curPrim["pen"]);
                            if (curPenPtr == nullptr)
                            {
                                // Если карандаш отсутствует или неверный
                                errCount++;
                                continue;
                            }

                            // Получение заливки
                            if (type == PrimitiveTypes::RectangleT || type == PrimitiveTypes::EllipseT || type == PrimitiveTypes::PolygonT)
                            {
                                curBrushPtr = jsonGetBrush(curPrim["brush"]);
                                if (curBrushPtr == nullptr)
                                {
                                    // Если заливка отсутствует или неверная
                                    errCount++;
                                    delete curPenPtr;
                                    continue;
                                }
                            }

                            // Получение точек
                            if (type == PrimitiveTypes::CurveT || type == PrimitiveTypes::PolygonT)
                            {
                                curPointsPtr = jsonGetPoints(curPrim["points"]);
                                if (curPointsPtr == nullptr)
                                {
                                    // Если точки отсутствуют или заданы неверно
                                    errCount++;
                                    delete curPenPtr;
                                    if (curBrushPtr != nullptr)
                                        delete curBrushPtr;
                                    continue;
                                }
                            }

                            // Получение значений, специфичных для конкретных классов
                            switch (type)
                            {
                            case PrimitiveTypes::PixelT: {
                                Pixel * obj = jsonGetPixel(rootArray[i]);
                                if (obj == nullptr)
                                {
                                    // Если значения пикселя неверные
                                    errCount++;
                                    delete curPenPtr;
                                    continue;
                                }
                                newPrimitives.append(obj);
                                break;
                            }
                            case PrimitiveTypes::LineT: {
                                Line * obj = jsonGetLine(curPrim);
                                if (obj == nullptr)
                                {
                                    // Если значения линии неверные
                                    errCount++;
                                    delete curPenPtr;
                                    continue;
                                }
                                newPrimitives.append(obj);
                                break;
                            }
                            case PrimitiveTypes::RectangleT: {
                                Rectangle * obj = jsonGetRectangle(curPrim);
                                if (obj == nullptr)
                                {
                                    // Если значения прямоугольника неверные
                                    errCount++;
                                    delete curPenPtr;
                                    continue;
                                }
                                newPrimitives.append(obj);
                                break;
                            }
                            case PrimitiveTypes::EllipseT: {
                                Ellipse * obj = jsonGetEllipse(rootArray[i]);
                                if (obj == nullptr)
                                {
                                    // Если значения эллипса неверные
                                    errCount++;
                                    delete curPenPtr;
                                    continue;
                                }
                                newPrimitives.append(obj);
                                break;
                            }
                            case PrimitiveTypes::ArcT: {
                                Arc * obj = jsonGetArc(rootArray[i]);
                                if (obj == nullptr)
                                {
                                    // Если значения дуги неверные
                                    errCount++;
                                    delete curPenPtr;
                                    continue;
                                }
                                newPrimitives.append(obj);
                                break;
                            }
                            case PrimitiveTypes::CurveT: {
                                newPrimitives.append(new Curve);
                                break;
                            }
                            case PrimitiveTypes::PolygonT: {
                                newPrimitives.append(new Polygon);
                                break;
                            }
                            }

                            // Вставление общих элементов в последний добавленный примитив
                            newPrimitives.last()->pen = curPenPtr;
                            newPrimitives.last()->brush = curBrushPtr;
                            newPrimitives.last()->points = curPointsPtr;
                        }

                        if (errCount > 0)
                        {
                            // Если какие-то из примитивов в файле имели некорректные значения
                            QMessageBox msgBox;
                            msgBox.setWindowTitle("Предупреждение");
                            msgBox.setText("В файле присутствуют неверные значения для " + QString::number(errCount) + " из " + QString::number(elemCount) + " элеметов" +
                                           + "\nПродолжить открытие файла (некорректные элементы не будут записаны)?");
                            msgBox.setIcon(QMessageBox::Warning);
                            QAbstractButton * yesButton = msgBox.addButton("Всё равно открыть", QMessageBox::YesRole);
                            msgBox.addButton("Отмена", QMessageBox::NoRole);
                            msgBox.exec();
                            if (msgBox.clickedButton() == yesButton)
                                errCount = 0;
                        }

                        if (errCount == 0)
                        {
                            // Если файл был изначально корректный или если пользователь согласил с получением хоть каких-то данных
                            genItemsFromClasses(&newPrimitives);
                        }
                    }
                }
                else
                {
                    // Если корневой элемент JSON - не список
                    QMessageBox::warning(this, "Ошибка", "Неверные данные в файле");
                }
            }
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        }
    }
}

void MainWindow::on_action_BIN_open_triggered()
{
    // Загрузка бинарных данных в таблицу
    QString filename = QFileDialog::getOpenFileName(this, "Открыть файл", DEF_FILE_DIR, tr("Binary files (*.bin)"));
    if (filename.isEmpty())
    {
        // Если не был выбран файл
        QMessageBox::warning(this, "Ошибка", "Файл не выбран");
    }
    else
    {
        QFile file(filename);
        if (file.open(QIODevice::ReadOnly))
        {
            // Считываем количество элементов
            char mas[intSz];
            int count;
            file.read(mas, intSz);
            memcpy(&count, &mas, intSz);
            if (count >= 0 && count < 100)
            {
                QVector<Primitive *> newPrimitives;
                bool okData = true;

                for (int i = 0; i < count; i++)
                {
                    // Считываем каждый отдельный элемент
                    int typeNum;
                    file.read(mas, intSz);
                    memcpy(&typeNum, &mas, intSz);
                    if (typeNum < 0 || typeNum > 6)
                    {
                        // Если в файле тип имеет неверное значение
                        okData = false;
                        break;
                    }
                    PrimitiveTypes type = static_cast<PrimitiveTypes>(typeNum);

                    bool okPrim = true;
                    switch (type)
                    {
                    // Пиксель
                    case PixelT: {
                        Pen * pen = binGetPen(file);
                        if (pen != nullptr)
                        {
                            if (file.bytesAvailable() < 8)
                            {
                                delete pen;
                                okPrim = false;
                            }
                            else
                            {
                                int x, y;
                                file.read(mas, intSz);
                                memcpy(&x, &mas, intSz);
                                file.read(mas, intSz);
                                memcpy(&y, &mas, intSz);
                                if (x >= 0 && y >= 0)
                                {
                                    Pixel * obj = new Pixel;
                                    obj->pen = pen;
                                    obj->setXY(x, y);
                                    newPrimitives.append(obj);
                                }
                                else
                                {
                                    delete pen;
                                    okPrim = false;
                                }
                            }
                        }
                        else
                        {
                            okPrim = false;
                        }
                        break;
                    }
                    // Линия
                    case LineT: {
                        Pen * pen = binGetPen(file);
                        if (pen != nullptr)
                        {
                            if (file.bytesAvailable() < 16)
                            {
                                delete pen;
                                okPrim = false;
                            }
                            else
                            {
                                int x1, y1, x2, y2;
                                file.read(mas, intSz);
                                memcpy(&x1, &mas, intSz);
                                file.read(mas, intSz);
                                memcpy(&y1, &mas, intSz);
                                file.read(mas, intSz);
                                memcpy(&x2, &mas, intSz);
                                file.read(mas, intSz);
                                memcpy(&y2, &mas, intSz);
                                if (x1 >= 0 && y1 >= 0 && x2 >= 0 && y2 >= 0)
                                {
                                    Line * obj = new Line;
                                    obj->pen = pen;
                                    obj->p1.setXY(x1, y1);
                                    obj->p2.setXY(x2, y2);
                                    newPrimitives.append(obj);
                                }
                                else
                                {
                                    delete pen;
                                    okPrim = false;
                                }
                            }
                        }
                        else
                        {
                            okPrim = false;
                        }
                        break;
                    }
                    // Прямоугольник
                    case RectangleT: {
                        Pen * pen = binGetPen(file);
                        if (pen != nullptr)
                        {
                            Brush * brush = binGetBrush(file);
                            if (brush != nullptr)
                            {
                                if (file.bytesAvailable() < 16)
                                {
                                    delete pen;
                                    delete brush;
                                    okPrim = false;
                                }
                                else
                                {
                                    int x1, y1, x2, y2;
                                    file.read(mas, intSz);
                                    memcpy(&x1, &mas, intSz);
                                    file.read(mas, intSz);
                                    memcpy(&y1, &mas, intSz);
                                    file.read(mas, intSz);
                                    memcpy(&x2, &mas, intSz);
                                    file.read(mas, intSz);
                                    memcpy(&y2, &mas, intSz);
                                    if (x1 >= 0 && y1 >= 0 && x2 >= 0 && y2 >= 0)
                                    {
                                        Rectangle * obj = new Rectangle;
                                        obj->pen = pen;
                                        obj->brush = brush;
                                        obj->p1.setXY(x1, y1);
                                        obj->p2.setXY(x2, y2);
                                        newPrimitives.append(obj);
                                    }
                                    else
                                    {
                                        delete pen;
                                        delete brush;
                                        okPrim = false;
                                    }
                                }
                            }
                            else
                            {
                                okPrim = false;
                                delete pen;
                            }
                        }
                        else
                        {
                            okPrim = false;
                        }
                        break;
                    }
                    // Эллипс
                    case EllipseT: {
                        Pen * pen = binGetPen(file);
                        if (pen != nullptr)
                        {
                            Brush * brush = binGetBrush(file);
                            if (brush != nullptr)
                            {
                                if (file.bytesAvailable() < 16)
                                {
                                    delete pen;
                                    delete brush;
                                    okPrim = false;
                                }
                                else
                                {
                                    int x, y, r1, r2;
                                    file.read(mas, intSz);
                                    memcpy(&x, &mas, intSz);
                                    file.read(mas, intSz);
                                    memcpy(&y, &mas, intSz);
                                    file.read(mas, intSz);
                                    memcpy(&r1, &mas, intSz);
                                    file.read(mas, intSz);
                                    memcpy(&r2, &mas, intSz);
                                    if (x >= 0 && y >= 0 && r1 >= 0 && r2 >= 0)
                                    {
                                        Ellipse * obj = new Ellipse;
                                        obj->pen = pen;
                                        obj->brush = brush;
                                        obj->setXY(x, y);
                                        obj->setRadius1(r1);
                                        obj->setRadius2(r2);
                                        newPrimitives.append(obj);
                                    }
                                    else
                                    {
                                        delete pen;
                                        delete brush;
                                        okPrim = false;
                                    }
                                }
                            }
                            else
                            {
                                okPrim = false;
                                delete pen;
                            }
                        }
                        else
                        {
                            okPrim = false;
                        }
                        break;
                    }
                    // Дуга
                    case ArcT: {
                        Pen * pen = binGetPen(file);
                        if (pen != nullptr)
                        {
                            if (file.bytesAvailable() < 24)
                            {
                                delete pen;
                                okPrim = false;
                            }
                            else
                            {
                                int x, y, r1, r2, a1, a2;
                                file.read(mas, intSz);
                                memcpy(&x, &mas, intSz);
                                file.read(mas, intSz);
                                memcpy(&y, &mas, intSz);
                                file.read(mas, intSz);
                                memcpy(&r1, &mas, intSz);
                                file.read(mas, intSz);
                                memcpy(&r2, &mas, intSz);
                                file.read(mas, intSz);
                                memcpy(&a1, &mas, intSz);
                                file.read(mas, intSz);
                                memcpy(&a2, &mas, intSz);
                                if (x >= 0 && y >= 0 && r1 >= 0 && r2 >= 0 &&
                                        a1 >= -360 && a1 <= 360 && a2 >= -360 && a2 <= 360)
                                {
                                    Arc * obj = new Arc;
                                    obj->pen = pen;
                                    obj->setXY(x, y);
                                    obj->setRadius1(r1);
                                    obj->setRadius2(r2);
                                    obj->setStartAngle(a1);
                                    obj->setSpanAngle(a2);
                                    newPrimitives.append(obj);
                                }
                                else
                                {
                                    delete pen;
                                    okPrim = false;
                                }
                            }
                        }
                        else
                        {
                            okPrim = false;
                        }
                        break;
                    }
                    // Ломаная
                    case CurveT: {
                        Pen * pen = binGetPen(file);
                        if (pen != nullptr)
                        {
                            QVector<Point> * points = binGetPoints(file);
                            if (points != nullptr && points->count() >= 2)
                            {
                                Curve * obj = new Curve;
                                obj->pen = pen;
                                obj->points = points;
                                newPrimitives.append(obj);
                            }
                            else
                            {
                                delete pen;
                                okPrim = false;
                            }
                        }
                        else
                        {
                            okPrim = false;
                        }
                        break;
                    }
                    // Многоугольник
                    case PolygonT: {
                        Pen * pen = binGetPen(file);
                        if (pen != nullptr)
                        {
                            Brush * brush = binGetBrush(file);
                            if (brush != nullptr)
                            {
                                QVector<Point> * points = binGetPoints(file);
                                if (points != nullptr && points->count() >= 3)
                                {
                                    Polygon * obj = new Polygon;
                                    obj->pen = pen;
                                    obj->brush = brush;
                                    obj->points = points;
                                    newPrimitives.append(obj);
                                }
                                else
                                {
                                    delete pen;
                                    delete brush;
                                    okPrim = false;
                                }
                            }
                            else
                            {
                                delete pen;
                                okPrim = false;
                            }
                        }
                        else
                        {
                            okPrim = false;
                        }
                        break;
                    }
                    }

                    if (!okPrim)
                    {
                        okData = false;
                        break;
                    }
                }

                if (okData)
                {
                    genItemsFromClasses(&newPrimitives);
                }
                else
                {
                    QMessageBox::warning(this, "Ошибка", "Файл повреждён");
                }
            }
            else
            {
                QMessageBox::warning(this, "Ошибка", "Файл повреждён");
            }

            // Закрываем файл
            file.close();
        }
    }
}


//-----------------------------
//- Функции для работы с JSON -
//-----------------------------

int MainWindow::jsonGetNum(QJsonValueRef val, bool * flag)
{
    // Проверка и получение целого числа из поля объекта
    if (val.isDouble())
    {
        if (flag != nullptr)
            *flag = true;
        return val.toInt();
    }
    else
    {
        if (flag != nullptr)
            *flag = false;
        return 0;
    }
}

QString MainWindow::jsonGetString(QJsonValueRef val, bool * flag)
{
    // Проверка и получение строки из поля объекта
    if (val.isString())
    {
        if (flag != nullptr)
            *flag = true;
        return val.toString();
    }
    else
    {
        if (flag != nullptr)
            *flag = false;
        return "";
    }
}

Color MainWindow::jsonGetColor(QJsonValueRef val, bool * flag)
{
    // Получение значений цвета из JSON объекта
    if (val.isObject())
    {
        QJsonObject colorObj = val.toObject();
        bool okRed, okGreen, okBlue, okAlpha;
        int red = jsonGetNum(colorObj["red"], &okRed);
        int green = jsonGetNum(colorObj["green"], &okGreen);
        int blue = jsonGetNum(colorObj["blue"], &okBlue);
        int alpha = jsonGetNum(colorObj["alpha"], &okAlpha);
        if (okRed && okGreen && okBlue && okAlpha)
        {
            Color newColor;
            bool okColor = newColor.setRGBA(red, green, blue, alpha);
            if (okColor)
            {
                if (flag != nullptr)
                    *flag = true;
                return newColor;
            }
        }
    }
    if (flag != nullptr)
        *flag = false;
    return Color();
}

Pen * MainWindow::jsonGetPen(QJsonValueRef val, bool * flag)
{
    // Создание класса карандаша из JSON объекта
    if (val.isObject())
    {
        QJsonObject penObj = val.toObject();
        bool okColor;
        Color color = jsonGetColor(penObj["color"], &okColor);
        if (okColor)
        {
            bool okWidth, okStyle;
            int width = jsonGetNum(penObj["width"], &okWidth);
            int style = jsonGetNum(penObj["style"], &okStyle);
            if (okWidth && okStyle)
            {
                Pen * newPen = new Pen;
                okWidth = newPen->setWeight(width);
                okStyle = newPen->setStyle(style);
                if (okWidth && okStyle)
                {
                    int r, g, b, a;
                    color.getRGBA(r, g, b, a);
                    newPen->setRGBA(r, g, b, a);
                    if (flag != nullptr)
                        *flag = true;
                    return newPen;
                }
                delete newPen;
            }
        }
    }
    if (flag != nullptr)
        *flag = false;
    return nullptr;
}

Brush * MainWindow::jsonGetBrush(QJsonValueRef val, bool * flag)
{
    // Создание класса заливки из JSON объекта
    if (val.isObject())
    {
        QJsonObject brushObj = val.toObject();
        bool okColor;
        Color color = jsonGetColor(brushObj["color"], &okColor);
        if (okColor)
        {
            bool okStyle;
            int style = jsonGetNum(brushObj["style"], &okStyle);
            if (okStyle)
            {
                Brush * newBrush = new Brush;
                okStyle = newBrush->setStyle(style);
                if (okStyle)
                {
                    int r, g, b, a;
                    color.getRGBA(r, g, b, a);
                    newBrush->setRGBA(r, g, b, a);
                    if (flag != nullptr)
                        *flag = true;
                    return newBrush;
                }
                delete newBrush;
            }
        }
    }
    if (flag != nullptr)
        *flag = false;
    return nullptr;
}

Point MainWindow::jsonGetPoint(QJsonValueRef val, bool * flag)
{
    // Создание класса точки из JSON объекта
    if (val.isObject())
    {
        QJsonObject pointObj = val.toObject();
        bool okX, okY;
        int x = jsonGetNum(pointObj["x"], &okX);
        int y = jsonGetNum(pointObj["y"], &okY);
        if (okX & okY)
        {
            Point newPoint;
            bool okPoint = newPoint.setXY(x, y);
            if (okPoint)
            {
                if (flag != nullptr)
                    *flag = true;
                return newPoint;
            }
        }
    }
    if (flag != nullptr)
        *flag = false;
    return Point();
}

QVector<Point> * MainWindow::jsonGetPoints(QJsonValueRef val, bool * flag)
{
    // Получения вектора точек из JSON списка
    if (val.isArray())
    {
        QJsonArray pointsArr = val.toArray();
        QVector<Point> * newPoints = new QVector<Point>;
        bool okArr = true;
        int count = pointsArr.count();
        for (int i = 0; i < count; i++)
        {
            bool okPoint;
            Point newPoint = jsonGetPoint(pointsArr[i], &okPoint);
            if (okPoint) newPoints->append(newPoint);
            else
            {
                okArr = false;
                break;
            }
        }
        if (okArr)
        {
            if (flag != nullptr)
                *flag = true;
            return newPoints;
        }
        else delete newPoints;
    }
    if (flag != nullptr)
        *flag = false;
    return nullptr;
}

Pixel * MainWindow::jsonGetPixel(QJsonValueRef val)
{
    // Создание класса пикселя из JSON объекта
    bool okPoint;
    Point coords = jsonGetPoint(val, &okPoint);
    if (okPoint)
    {
        Pixel * result = new Pixel;
        result->setXY(coords.getX(), coords.getY());
        return result;
    }
    return nullptr;
}

Line * MainWindow::jsonGetLine(QJsonObject val)
{
    // Создание класса линии из JSON объекта
    bool okStart, okEnd;
    Point start = jsonGetPoint(val["start"], &okStart);
    Point end = jsonGetPoint(val["end"], &okEnd);
    if (okStart && okEnd)
    {
        Line * result = new Line;
        result->p1 = start;
        result->p2 = end;
        return result;
    }
    return nullptr;
}

Rectangle * MainWindow::jsonGetRectangle(QJsonObject val)
{
    // Создание класса прямоугольника из JSON объекта
    bool okStart, okEnd;
    Point start = jsonGetPoint(val["start"], &okStart);
    Point end = jsonGetPoint(val["end"], &okEnd);
    if (okStart && okEnd)
    {
        Rectangle * result = new Rectangle;
        result->p1 = start;
        result->p2 = end;
        return result;
    }
    return nullptr;
}

Ellipse * MainWindow::jsonGetEllipse(QJsonValueRef val)
{
    // Создание класса эллипса из JSON объекта
    QJsonObject primObj = val.toObject();
    bool okPoint;
    Point coords = jsonGetPoint(val, &okPoint);
    if (okPoint)
    {
        bool okRadius1, okRadius2;
        int radius1 = jsonGetNum(primObj["radius1"], &okRadius1);
        int radius2 = jsonGetNum(primObj["radius2"], &okRadius2);
        if (okRadius1 && okRadius2)
        {
            Ellipse * result = new Ellipse;
            result->setXY(coords.getX(), coords.getY());
            result->setRadius1(radius1);
            result->setRadius2(radius2);
            return result;
        }
    }
    return nullptr;
}

Arc * MainWindow::jsonGetArc(QJsonValueRef val)
{
    // Создание класса дуги из JSON объекта
    QJsonObject primObj = val.toObject();
    bool okPoint;
    Point coords = jsonGetPoint(val, &okPoint);
    if (okPoint)
    {
        bool okRadius1, okRadius2, okStart, okSpan;
        int radius1 = jsonGetNum(primObj["radius1"], &okRadius1);
        int radius2 = jsonGetNum(primObj["radius2"], &okRadius2);
        int startAngle = jsonGetNum(primObj["start_angle"], &okStart);
        int spanAngle = jsonGetNum(primObj["span_angle"], &okSpan);
        if (okRadius1 && okRadius2 && okStart && okSpan)
        {
            Arc * result = new Arc;
            result->setXY(coords.getX(), coords.getY());
            result->setRadius1(radius1);
            result->setRadius2(radius2);
            result->setStartAngle(startAngle);
            result->setSpanAngle(spanAngle);
            return result;
        }
    }
    return nullptr;
}


//------------------------------------------
//- Функции для работы с бинарными файлами -
//------------------------------------------

Pen * MainWindow::binGetPen(QFile &file, bool * flag)
{
    // Получение карандаша из файла
    char mas[intSz];
    bool ok = true;
    Pen * pen = new Pen;
    if (file.bytesAvailable() < 24) ok = false;
    if (ok)
    {
        int red, green, blue, alpha, width, style;
        file.read(mas, intSz);
        memcpy(&red, &mas, intSz);
        file.read(mas, intSz);
        memcpy(&green, &mas, intSz);
        file.read(mas, intSz);
        memcpy(&blue, &mas, intSz);
        file.read(mas, intSz);
        memcpy(&alpha, &mas, intSz);
        file.read(mas, intSz);
        memcpy(&width, &mas, intSz);
        file.read(mas, intSz);
        memcpy(&style, &mas, intSz);

        if (ok) ok = pen->setRGBA(red, green, blue, alpha);
        if (ok) ok = pen->setWeight(width);
        if (ok) ok = pen->setStyle(style);
    }

    if (!ok)
    {
        delete pen;
        if (flag != nullptr)
            *flag = false;
        return nullptr;
    }
    if (flag != nullptr)
        *flag = false;
    return pen;
}

Brush * MainWindow::binGetBrush(QFile &file, bool * flag)
{
    // Получение заливки из файла
    char mas[intSz];
    bool ok = true;
    Brush * brush = new Brush;
    if (file.bytesAvailable() < 20) ok = false;
    if (ok)
    {
        int red, green, blue, alpha, style;
        file.read(mas, intSz);
        memcpy(&red, &mas, intSz);
        file.read(mas, intSz);
        memcpy(&green, &mas, intSz);
        file.read(mas, intSz);
        memcpy(&blue, &mas, intSz);
        file.read(mas, intSz);
        memcpy(&alpha, &mas, intSz);
        file.read(mas, intSz);
        memcpy(&style, &mas, intSz);

        if (ok) ok = brush->setRGBA(red, green, blue, alpha);
        if (ok) ok = brush->setStyle(style);
    }

    if (!ok)
    {
        delete brush;
        if (flag != nullptr)
            *flag = false;
        return nullptr;
    }
    if (flag != nullptr)
        *flag = false;
    return brush;
}

QVector<Point> * MainWindow::binGetPoints(QFile &file, bool * flag)
{
    // Получение вектора точек
    char mas[intSz];
    bool ok = true;
    QVector<Point> * points = new QVector<Point>;

    int count;
    file.read(mas, intSz);
    memcpy(&count, &mas, intSz);
    if (count <= 0 || file.bytesAvailable() < count * 8) ok = false;

    if (ok)
    {
        bool okArr = true;
        for (int i = 0; i < count; i++)
        {
            int x, y;
            Point newPoint;

            file.read(mas, intSz);
            memcpy(&x, &mas, intSz);
            file.read(mas, intSz);
            memcpy(&y, &mas, intSz);

            okArr = newPoint.setXY(x, y);
            if (!okArr)
            {
                ok = false;
                break;
            }
            points->append(newPoint);
        }
    }

    if (!ok)
    {
        delete points;
        if (flag != nullptr)
            *flag = false;
        return nullptr;
    }
    if (flag != nullptr)
        *flag = false;
    return points;
}
