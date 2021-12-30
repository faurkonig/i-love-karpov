#ifndef CURVEDIALOG_H
#define CURVEDIALOG_H

#include <QDialog>
#include <QColorDialog>

#include "pen.h"
#include "brush.h"
#include "point.h"


namespace Ui {
class CurveDialog;
}

class CurveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CurveDialog(QWidget *parent = nullptr);
    ~CurveDialog();

    bool setCurve(int *cbC);
    bool setPolygon(int *cbC);

    Point **pointerToPoints;
    Pen *tpen;
    Brush *tbrush;

private slots:
    void on_pushButtonPenColor_clicked();

    void on_comboBoxPenStyle_activated(int index);

    void on_spinBoxPenWeight_valueChanged(int arg1);

    void on_spinBoxCount_valueChanged(int arg1);

    void on_pushButtonCancel_clicked();

    void on_pushButtonDraw_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_pushButtonBrushColor_clicked();

    void on_comboBoxBrushStyle_activated(int index);

private:
    Ui::CurveDialog *ui;

    QColor defaultBackgroundColor;
    bool autoChange = false;

    int *cbCount;

    bool drawOk = false;
};

#endif // CURVEDIALOG_H
