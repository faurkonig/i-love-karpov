#include "tooldialog.h"
#include "ui_tooldialog.h"

/// Конструктор
ToolDialog::ToolDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ToolDialog)
{
    ui->setupUi(this);

    ui->labelError->hide();

    defStyle = ui->lineEditX1->palette();
    redStyle = ui->lineEditX1->palette();
    redStyle.setColor(QPalette::Base, Qt::red);
}

/// Деструктор
ToolDialog::~ToolDialog()
{
    delete ui;
}

/// Открытие окна для рисования пикселя
bool ToolDialog::setPixel(int *cb_x, int *cb_y)
{
    toolId = 0;
    x1 = cb_x;
    y1 = cb_y;

    ui->labelTool->setText("Инструмент: пиксель");
    QPalette pal = ui->pushButtonPenColor->palette();
    int rr, gg, bb, aa;
    tpen->getRGBA(rr, gg, bb, aa);
    pal.setColor(QPalette::Button, QColor(rr, gg, bb, aa));
    ui->pushButtonPenColor->setPalette(pal);
    ui->labelX2->hide();
    ui->labelY2->hide();
    ui->lineEditX2->hide();
    ui->lineEditY2->hide();
    ui->pushButtonBrushColor->hide();
    ui->labelBrushStyle->hide();
    ui->comboBoxBrushStyle->hide();
    ui->labelPenStyle->hide();
    ui->comboBoxPenStyle->hide();
    ui->labelA1->hide();
    ui->labelA2->hide();
    ui->lineEditA1->hide();
    ui->lineEditA2->hide();

    exec();
    return drawOk;
}

/// Открытие окна для рисования линии
bool ToolDialog::setLine(int *cb_x1, int *cb_y1, int *cb_x2, int *cb_y2)
{
    toolId = 1;
    x1 = cb_x1;
    y1 = cb_y1;
    x2 = cb_x2;
    y2 = cb_y2;

    ui->labelTool->setText("Инструмент: линия");
    QPalette pal = ui->pushButtonPenColor->palette();
    int rr, gg, bb, aa;
    tpen->getRGBA(rr, gg, bb, aa);
    pal.setColor(QPalette::Button, QColor(rr, gg, bb, aa));
    ui->pushButtonPenColor->setPalette(pal);
    ui->pushButtonBrushColor->hide();
    ui->labelBrushStyle->hide();
    ui->comboBoxBrushStyle->hide();
    ui->labelX1->setText("X1");
    ui->labelY1->setText("Y1");
    ui->labelA1->hide();
    ui->labelA2->hide();
    ui->lineEditA1->hide();
    ui->lineEditA2->hide();

    exec();
    return drawOk;
}

/// Открытие окна для рисования прямоугольника
bool ToolDialog::setRect(int *cb_x1, int *cb_y1, int *cb_x2, int *cb_y2)
{
    toolId = 2;
    x1 = cb_x1;
    y1 = cb_y1;
    x2 = cb_x2;
    y2 = cb_y2;

    ui->labelTool->setText("Инструмент: прямоугольник");
    QPalette pal = ui->pushButtonPenColor->palette();
    int rr, gg, bb, aa;
    tpen->getRGBA(rr, gg, bb, aa);
    pal.setColor(QPalette::Button, QColor(rr, gg, bb, aa));
    ui->pushButtonPenColor->setPalette(pal);
    tbrush->getRGBA(rr, gg, bb, aa);
    pal.setColor(QPalette::Button, QColor(rr, gg, bb, aa));
    ui->pushButtonBrushColor->setPalette(pal);
    ui->labelX1->setText("X1");
    ui->labelY1->setText("Y1");
    ui->labelA1->hide();
    ui->labelA2->hide();
    ui->lineEditA1->hide();
    ui->lineEditA2->hide();

    exec();
    return drawOk;
}

/// Открытие окна для рисования эллипса
bool ToolDialog::setEllips(int *cb_x1, int *cb_y1, int *cb_x2, int *cb_y2)
{
    toolId = 3;
    x1 = cb_x1;
    y1 = cb_y1;
    x2 = cb_x2;
    y2 = cb_y2;

    ui->labelTool->setText("Инструмент: эллипс");
    QPalette pal = ui->pushButtonPenColor->palette();
    int rr, gg, bb, aa;
    tpen->getRGBA(rr, gg, bb, aa);
    pal.setColor(QPalette::Button, QColor(rr, gg, bb, aa));
    ui->pushButtonPenColor->setPalette(pal);
    tbrush->getRGBA(rr, gg, bb, aa);
    pal.setColor(QPalette::Button, QColor(rr, gg, bb, aa));
    ui->pushButtonBrushColor->setPalette(pal);
    ui->labelX2->setText("R1");
    ui->labelY2->setText("R2");
    ui->labelA1->hide();
    ui->labelA2->hide();
    ui->lineEditA1->hide();
    ui->lineEditA2->hide();

    exec();
    return drawOk;
}

/// Открытие окна для рисования дуги
bool ToolDialog::setArc(int *cb_x1, int *cb_y1, int *cb_x2, int *cb_y2, int *cb_a1, int *cb_a2)
{
    toolId = 4;
    x1 = cb_x1;
    y1 = cb_y1;
    x2 = cb_x2;
    y2 = cb_y2;
    a1 = cb_a1;
    a2 = cb_a2;

    ui->labelTool->setText("Инструмент: кривая");
    QPalette pal = ui->pushButtonPenColor->palette();
    int rr, gg, bb, aa;
    tpen->getRGBA(rr, gg, bb, aa);
    pal.setColor(QPalette::Button, QColor(rr, gg, bb, aa));
    ui->pushButtonPenColor->setPalette(pal);
    ui->pushButtonBrushColor->hide();
    ui->labelBrushStyle->hide();
    ui->comboBoxBrushStyle->hide();
    ui->labelX2->setText("R1");
    ui->labelY2->setText("R2");

    exec();
    return drawOk;
}

/// Открытие окна для рисования эллипса
bool ToolDialog::setFace(int *cb_x1, int *cb_y1, int *cb_x2, int *cb_y2)
{
    toolId = 5;
    x1 = cb_x1;
    y1 = cb_y1;
    x2 = cb_x2;
    y2 = cb_y2;

    ui->labelTool->setText("Инструмент: рожица");
    QPalette pal = ui->pushButtonPenColor->palette();
    int rr, gg, bb, aa;
    tpen->getRGBA(rr, gg, bb, aa);
    pal.setColor(QPalette::Button, QColor(rr, gg, bb, aa));
    ui->pushButtonPenColor->setPalette(pal);
    ui->pushButtonBrushColor->hide();
    ui->labelBrushStyle->hide();
    ui->comboBoxBrushStyle->hide();
    ui->labelX2->setText("R1");
    ui->labelY2->setText("R2");
    ui->labelA1->hide();
    ui->labelA2->hide();
    ui->lineEditA1->hide();
    ui->lineEditA2->hide();

    exec();
    return drawOk;
}

/// Установка цвета линии
void ToolDialog::on_pushButtonPenColor_clicked()
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
void ToolDialog::on_pushButtonBrushColor_clicked()
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

/// Нажатие кнопки "Отмена"
void ToolDialog::on_pushButtonCancel_clicked()
{
    close();
}

/// Нажатие кнопки "Рисовать"
void ToolDialog::on_pushButtonDraw_clicked()
{
    ui->lineEditX1->setPalette(defStyle);
    ui->lineEditY1->setPalette(defStyle);
    ui->lineEditX2->setPalette(defStyle);
    ui->lineEditY2->setPalette(defStyle);
    ui->lineEditA1->setPalette(defStyle);
    ui->lineEditA2->setPalette(defStyle);
    ui->labelError->hide();

    // Получение первых 2 значения
    int nx1 = -1, ny1 = -1, nx2 = -1, ny2 = -1, na1 = -1, na2 = -1;
    bool oks[6] = {true, true, true, true, true, true};

    nx1 = ui->lineEditX1->text().toInt(&oks[0]);
    ny1 = ui->lineEditY1->text().toInt(&oks[1]);
    if (nx1 < 0) oks[0] = false;
    if (ny1 < 0) oks[1] = false;
    if (toolId != 0)
    {
        nx2 = ui->lineEditX2->text().toInt(&oks[2]);
        ny2 = ui->lineEditY2->text().toInt(&oks[3]);
        if (nx2 < 0) oks[2] = false;
        if (ny2 < 0) oks[3] = false;
        if (toolId == 4)
        {
            na1 = ui->lineEditA1->text().toInt(&oks[4]);
            na2 = ui->lineEditA2->text().toInt(&oks[5]);
            if (na1 < -360 || na1 > 360) oks[4] = false;
            if (na2 < -360 || na2 > 360) oks[5] = false;
        }
    }

    if (!oks[0]) ui->lineEditX1->setPalette(redStyle);
    if (!oks[1]) ui->lineEditY1->setPalette(redStyle);
    if (!oks[2]) ui->lineEditX2->setPalette(redStyle);
    if (!oks[3]) ui->lineEditY2->setPalette(redStyle);
    if (!oks[4]) ui->lineEditA1->setPalette(redStyle);
    if (!oks[5]) ui->lineEditA2->setPalette(redStyle);

    if (oks[0] && oks[1] && oks[2] && oks[3] && oks[4] && oks[5])
    {
        // Возвращение первых значений в класс
        *x1 = nx1;
        *y1 = ny1;
        if (toolId != 0)
        {
            *x2 = nx2;
            *y2 = ny2;
        }
        if (toolId == 4)
        {
            *a1 = na1;
            *a2 = na2;
        }
        drawOk = true;
        close();
    }
    else
    {
        ui->labelError->show();
    }
}

/// Установка стиля линий
void ToolDialog::on_comboBoxPenStyle_activated(int index)
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

/// Установка ширины линий
void ToolDialog::on_spinBoxPenWeight_valueChanged(int nw)
{
    tpen->setWidth(nw);
}

/// Установка стиля заливки
void ToolDialog::on_comboBoxBrushStyle_activated(int index)
{
    tbrush->setStyle(index);
    if (index == 0) ui->pushButtonBrushColor->hide();
    else ui->pushButtonBrushColor->show();
}

/// Изменение текста в поле ввода для X1
void ToolDialog::on_lineEditX1_textChanged(const QString &)
{
    ui->lineEditX1->setPalette(defStyle);
}

/// Изменение текста в поле ввода для Y1
void ToolDialog::on_lineEditY1_textChanged(const QString &)
{
    ui->lineEditY1->setPalette(defStyle);
}

/// Изменение текста в поле ввода для X2
void ToolDialog::on_lineEditX2_textChanged(const QString &)
{
    ui->lineEditX2->setPalette(defStyle);
}

/// Изменение текста в поле ввода для Y2
void ToolDialog::on_lineEditY2_textChanged(const QString &)
{
    ui->lineEditY2->setPalette(defStyle);
}

/// Изменение текста в поле ввода для A1
void ToolDialog::on_lineEditA1_textChanged(const QString &)
{
    ui->lineEditA1->setPalette(defStyle);
}

/// Изменение текста в поле ввода для A2
void ToolDialog::on_lineEditA2_textChanged(const QString &)
{
    ui->lineEditA2->setPalette(defStyle);
}
