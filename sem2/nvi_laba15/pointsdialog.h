#ifndef POINTSDIALOG_H
#define POINTSDIALOG_H

#include <QDialog>
#include <QVector>

#include <point.h>


namespace Ui {
class PointsDialog;
}

class PointsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PointsDialog(QWidget *parent = nullptr, QColor defColor = QColor(), QColor errColor = QColor());
    ~PointsDialog();

    void openDialog(QString title, QVector<Point> * newPointsPtr, bool isCurve = false);

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_tableWidget_cellChanged(int row, int column);

    void on_pushButtonOk_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::PointsDialog *ui;

    QVector<Point> * pointsPtr = nullptr;

    QColor defaultColor;
    QColor errorColor;
    bool isAutoChange = false;
};

#endif // POINTSDIALOG_H
