#include "curvedialog.h"
#include "ui_curvedialog.h"

CurveDialog::CurveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurveDialog)
{
    // Конструктор
    ui->setupUi(this);

    // Нахождение "нормального" цвета для фона клеток таблицы
    defaultBackgroundColor = ui->tableWidget->palette().color(QPalette::Base);
}

CurveDialog::~CurveDialog()
{
    // Деструктор
    delete ui;
}

bool CurveDialog::setCurve(int *cbC)
{
    // Открытие окна для ломанной
    ui->labelTool->setText("Инструмент: ломанная");
    cbCount = cbC;
    ui->pushButtonBrushColor->hide();
    ui->labelBrushStyle->hide();
    ui->comboBoxBrushStyle->hide();
//    int rr, gg, bb, aa;
//    tpen->getRGBA(rr, gg, bb, aa);
//    pal.setColor(QPalette::Button, QColor(rr, gg, bb, aa));
//    ui->pushButtonPenColor->setPalette(pal);

    exec();
    return drawOk;
}

bool CurveDialog::setPolygon(int *cbC)
{
    // Открытие окна для многоугольника
    ui->labelTool->setText("Инструмент: многоугольник");
    cbCount = cbC;

    exec();
    return drawOk;
}

void CurveDialog::on_spinBoxCount_valueChanged(int count)
{
    // Изменение количества строк в таблице
    ui->tableWidget->setRowCount(count);
}

void CurveDialog::on_pushButtonPenColor_clicked()
{
    // Установка цвета линии
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

void CurveDialog::on_pushButtonBrushColor_clicked()
{
    // Установка цвета заливки
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

void CurveDialog::on_comboBoxPenStyle_activated(int index)
{
    // Установка стиля границ
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

void CurveDialog::on_spinBoxPenWeight_valueChanged(int nw)
{
    // Установка толщины границ
    tpen->setWidth(nw);
}

void CurveDialog::on_comboBoxBrushStyle_activated(int index)
{
    // Установка стиля заливки
    tbrush->setStyle(index);
    if (index == 0) ui->pushButtonBrushColor->hide();
    else ui->pushButtonBrushColor->show();
}

void CurveDialog::on_pushButtonCancel_clicked()
{
    // Нажатие кнопки "Отмена"
    close();
}

void CurveDialog::on_pushButtonDraw_clicked()
{
    // Нажатие кнопки "Рисовать"
    autoChange = true;

    // Получение кол-ва строк из формы
    int count = ui->spinBoxCount->value();
    *cbCount = count;

    // Инициализация массива
    *pointerToPoints = new point[count];

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
        }
        else
        {
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

void CurveDialog::on_tableWidget_cellChanged(int row, int column)
{
    // Если были введены данные в ячейку
    if (!autoChange)
        ui->tableWidget->item(row, column)->setBackground(defaultBackgroundColor);
}
