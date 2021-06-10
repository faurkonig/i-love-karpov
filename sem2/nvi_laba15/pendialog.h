#ifndef PENDIALOG_H
#define PENDIALOG_H

#include <QDialog>
#include <QColorDialog>

#include <pen.h>


namespace Ui {
class PenDialog;
}

class PenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PenDialog(QWidget *parent = nullptr);
    ~PenDialog();

    void openDialog(QString title, Pen * newPenPtr, bool isPixel = false);

private slots:
    void on_comboBoxStyle_activated(int index);

    void on_spinBoxWeight_valueChanged(int arg1);

    void on_pushButtonColor_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::PenDialog *ui;

    QPalette buttonPalette;

    Pen thisPen;
    Pen * penPtr;
    bool thisIsPixel;
};

#endif // PENDIALOG_H
