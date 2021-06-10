#include "pointsdialog.h"
#include "ui_pointsdialog.h"

PointsDialog::PointsDialog(QWidget *parent, QColor defColor, QColor errColor) :
    QDialog(parent),
    ui(new Ui::PointsDialog)
{
    ui->setupUi(this);

    defaultColor = defColor;
    errorColor = errColor;
}

void PointsDialog::openDialog(QString title, QVector<Point> * newPointsPtr, bool isCurve)
{
    // Открытие диалога
    // Применяем входные значения
    ui->labelHeader->setText("Инструмент: " + title);
    pointsPtr = newPointsPtr;

    // У ломаной может быть 2 точки
    if (isCurve)
        ui->spinBox->setMinimum(2);

    // Ставим в таблицу уже существующие значения
    int count = pointsPtr->count();
    ui->spinBox->setValue(count);
    isAutoChange = true;
    QTableWidgetItem * twi = nullptr;
    for (int i = 0; i < count; i++) {
        twi = new QTableWidgetItem;
        twi->setText(QString::number((*pointsPtr)[i].getX()));
        ui->tableWidget->setItem(i, 0, twi);
        twi = new QTableWidgetItem;
        twi->setText(QString::number((*pointsPtr)[i].getY()));
        ui->tableWidget->setItem(i, 1, twi);
    }
    isAutoChange = false;

    // Открываем диалог
    exec();
}

PointsDialog::~PointsDialog()
{
    delete ui;
}

void PointsDialog::on_spinBox_valueChanged(int arg1)
{
    // Изменение количества строк в таблице
    ui->tableWidget->setRowCount(arg1);
}


void PointsDialog::on_tableWidget_cellChanged(int row, int column)
{
    // Обработчик изменения ячеек в таблице
    if (!isAutoChange)
        ui->tableWidget->item(row, column)->setBackground(defaultColor);
}

void PointsDialog::on_pushButtonOk_clicked()
{
    // ОК
    // Проверка всей таблицы значений
    isAutoChange = true;
    bool okArr = true, ok = true;
    QTableWidgetItem * twi = nullptr;
    int count = ui->spinBox->value();
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            ok = true;
            twi = ui->tableWidget->item(i, j);
            if (twi == nullptr)
            {
                twi = new QTableWidgetItem;
                ui->tableWidget->setItem(i, j, twi);
                ok = false;
            }
            else
            {
                int val = twi->text().toInt(&ok);
                if (ok && val < 0) ok = false;
            }
            if (ok)
            {
                twi->setBackground(defaultColor);
            }
            else
            {
                okArr = false;
                twi->setBackground(errorColor);
            }
        }
    }
    isAutoChange = false;

    if (okArr)
    {
        // Если все значения в таблице правильные, то записываем значения в вектор
        if ((*pointsPtr).count() != count)
            *pointsPtr = QVector<Point>(count);

        for (int i = 0; i < count; i++) {
            int x = ui->tableWidget->item(i, 0)->text().toInt();
            int y = ui->tableWidget->item(i, 1)->text().toInt();
            (*pointsPtr)[i].setXY(x, y);
        }
        // Закрываем диалог
        close();
    }
}


void PointsDialog::on_pushButtonCancel_clicked()
{
    // Отмена
    close();
}

