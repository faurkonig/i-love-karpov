#include "curvedialog.h"
#include "ui_curvedialog.h"

/// Конструктор
CurveDialog::CurveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurveDialog)
{
    ui->setupUi(this);

    // Нахождение "нормального" цвета для фона клеток таблицы
    defaultBackgroundColor = ui->tableWidget->palette().color(QPalette::Base);

    ui->labelError->hide();
}

/// Деструктор
CurveDialog::~CurveDialog()
{
    delete ui;
}

/// Открытие окна для ломанной
bool CurveDialog::setCurve(int *cbC)
{
    ui->labelTool->setText("Инструмент: ломанная");
    cbCount = cbC;
    ui->pushButtonBrushColor->hide();
    ui->labelBrushStyle->hide();
    ui->comboBoxBrushStyle->hide();

    exec();
    return drawOk;
}

/// Открытие окна для многоугольника
bool CurveDialog::setPolygon(int *cbC)
{
    ui->labelTool->setText("Инструмент: многоугольник");
    cbCount = cbC;

    exec();
    return drawOk;
}

/// Изменение количества строк в таблице
void CurveDialog::on_spinBoxCount_valueChanged(int count)
{
    ui->tableWidget->setRowCount(count);
}

/// Установка цвета линии
void CurveDialog::on_pushButtonPenColor_clicked()
{
    int r, g, b, a;
    tpen->getRGBA(r, g, b, a);
    QColor new_clr = QColorDialog::getColor(QColor(r, g, b, a), this, "Выбор цвета");

    if (new_clr.isValid())
    {
        QPalette pal = ui->pushButtonPenColor->palette();
        pal.setColor(QPalette::Button, new_clr);
        ui->pushButtonPenColor->setPalette(pal);

        new_clr.getRgb(&r, &g, &b, &a);
        tpen->setRGBA(r, g, b, a);
    }
}

/// Установка цвета заливки
void CurveDialog::on_pushButtonBrushColor_clicked()
{
    int r, g, b, a;
    tbrush->getRGBA(r, g, b, a);
    QColor new_clr = QColorDialog::getColor(QColor(r, g, b, a), this, "Выбор цвета");

    if (new_clr.isValid())
    {
        QPalette pal = ui->pushButtonBrushColor->palette();
        pal.setColor(QPalette::Button, new_clr);
        ui->pushButtonBrushColor->setPalette(pal);

        new_clr.getRgb(&r, &g, &b, &a);
        tbrush->setRGBA(r, g, b, a);
    }
}

/// Установка стиля границ
void CurveDialog::on_comboBoxPenStyle_activated(int index)
{
    tpen->setStyle(index);
    if (index == 0)
    {
        ui->pushButtonPenColor->hide();
        ui->labelPenWeight->hide();
        ui->spinBoxPenWeight->hide();
    }
    else
    {
        ui->pushButtonPenColor->show();
        ui->labelPenWeight->show();
        ui->spinBoxPenWeight->show();
    }
}

/// Установка толщины границ
void CurveDialog::on_spinBoxPenWeight_valueChanged(int nw)
{
    tpen->setWidth(nw);
}

/// Установка стиля заливки
void CurveDialog::on_comboBoxBrushStyle_activated(int index)
{
    tbrush->setStyle(index);
    if (index == 0) ui->pushButtonBrushColor->hide();
    else ui->pushButtonBrushColor->show();
}

/// Нажатие кнопки "Отмена"
void CurveDialog::on_pushButtonCancel_clicked()
{
    close();
}

/// Нажатие кнопки "Рисовать"
void CurveDialog::on_pushButtonDraw_clicked()
{
    autoChange = true;

    // Получение кол-ва строк из формы
    int count = ui->spinBoxCount->value();
    *cbCount = count;

    // Инициализация массива
    *pointerToPoints = new Point[count];

    // Флаги корректности ячейки для X, для Y и таблицы в целом
    bool okCell1 = true, okCell2 = true, okArr = true;
    int cx = -1, cy = -1;
    // Получение строк из формы
    for (int i = 0; i < count; i++)
    {
        if (ui->tableWidget->item(i, 0) != nullptr)
        {
            cx = ui->tableWidget->item(i, 0)->text().toInt(&okCell1);
            ui->tableWidget->item(i, 0)->setBackground(defaultBackgroundColor);

            if (cx < 0) okCell1 = false;
        }
        else
        {
            okCell1 = false;
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem);
        }

        if (ui->tableWidget->item(i, 1) != nullptr)
        {
            cy = ui->tableWidget->item(i, 1)->text().toInt(&okCell2);
            ui->tableWidget->item(i, 1)->setBackground(defaultBackgroundColor);

            if (cy < 0) okCell2 = false;
        }
        else
        {
            okCell2 = false;
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem);
        }

        if (okCell1 && okCell2)
        {
            (*pointerToPoints)[i].setXY(cx, cy);
            ui->labelError->hide();
        }
        else
        {
            ui->labelError->show();
            okArr = false;

            if (!okCell1)
                ui->tableWidget->item(i, 0)->setBackground(Qt::red);
            if (!okCell2)
                ui->tableWidget->item(i, 1)->setBackground(Qt::red);
        }
    }

    if (okArr)
    {
        // Если всё со всеми данными ок
        drawOk = true;
        close();
    }

    autoChange = false;
}

/// Если были введены данные в ячейку
void CurveDialog::on_tableWidget_cellChanged(int row, int column)
{
    if (!autoChange)
        ui->tableWidget->item(row, column)->setBackground(defaultBackgroundColor);
}
