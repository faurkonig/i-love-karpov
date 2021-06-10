#ifndef BRUSHDIALOG_H
#define BRUSHDIALOG_H

#include <QDialog>
#include <QColorDialog>

#include <brush.h>


namespace Ui {
class BrushDialog;
}

class BrushDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BrushDialog(QWidget *parent = nullptr);
    ~BrushDialog();

    void openDialog(QString title, Brush * newBrushPtr);

private slots:
    void on_comboBoxStyle_activated(int index);

    void on_pushButtonColor_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::BrushDialog *ui;

    QPalette buttonPalette;

    Brush thisBrush;
    Brush * brushPtr;
};

#endif // BRUSHDIALOG_H
