#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MAX_ARR 100000 // Макс. кол-во значений в таблице
#define RAND_OFF 200 // Разница для рандома -RAND_OFF/2 - минимально возможное, RAND_OFF - максимально возможное

#include <QMainWindow>
#include<QDebug>
#include<QElapsedTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_tableWidget_cellChanged(int row, int column);

    void on_pushButtonRandom_clicked();

    void on_pushButtonSum_clicked();

    void on_pushButtonMin_clicked();

    void on_pushButtonMax_clicked();

    void on_pushButtonAverage_clicked();

    void on_pushButtonSort1_clicked();

    void on_pushButtonSort2_clicked();

    void on_pushButtonSort3_clicked();

    void on_pushButtonSort4_clicked();

    void on_pushButtonSort5_clicked();

    void on_pushButtonCopies_clicked();

    void on_pushButtonSearch1_clicked();

    void on_pushButtonSearch2_clicked();

    void on_pushButtonEven_clicked();

private:
    Ui::MainWindow *ui;
    bool noAutoChange;

    QColor defaultBackgroundColor;
    QPalette redLineEditPalette;
    QPalette defaultLineEditPalette;
};
#endif // MAINWINDOW_H
