#ifndef TOOLDIALOG_H
#define TOOLDIALOG_H

#include <QDialog>
#include <QColorDialog>

#include "pen.h"
#include "brush.h"


namespace Ui {
class ToolDialog;
}

class ToolDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ToolDialog(QWidget *parent = nullptr);
    ~ToolDialog();

    bool setPixel(int *cb_x, int *cb_y);
    bool setLine(int *cb_x1, int *cb_y1, int *cb_x2, int *cb_y2);
    bool setRect(int *cb_x1, int *cb_y1, int *cb_x2, int *cb_y2);
    bool setEllips(int *cb_x1, int *cb_y1, int *cb_x2, int *cb_y2);
    bool setArc(int *cb_x1, int *cb_y1, int *cb_x2, int *cb_y2, int *cb_a1, int *cb_a2);
    bool setFace(int *cb_x1, int *cb_y1, int *cb_x2, int *cb_y2);

    Pen *tpen;
    Brush *tbrush;

private slots:

    void on_pushButtonCancel_clicked();

    void on_pushButtonDraw_clicked();

    void on_pushButtonPenColor_clicked();

    void on_pushButtonBrushColor_clicked();

    void on_comboBoxPenStyle_activated(int index);

    void on_spinBoxPenWeight_valueChanged(int arg1);

    void on_comboBoxBrushStyle_activated(int index);

    void on_lineEditX1_textChanged(const QString &arg1);

    void on_lineEditY1_textChanged(const QString &arg1);

    void on_lineEditX2_textChanged(const QString &arg1);

    void on_lineEditY2_textChanged(const QString &arg1);

    void on_lineEditA1_textChanged(const QString &arg1);

    void on_lineEditA2_textChanged(const QString &arg1);

private:
    Ui::ToolDialog *ui;

    // 0 - пиксель; 1 - линия; 2 - прямоугольник; 3 - эллипс; 4 - кривая
    int toolId;

    int *x1 = nullptr;
    int *y1 = nullptr;
    int *x2 = nullptr;
    int *y2 = nullptr;
    int *a1 = nullptr;
    int *a2 = nullptr;

    bool drawOk = false;

    QPalette defStyle;
    QPalette redStyle;
};

#endif // TOOLDIALOG_H
