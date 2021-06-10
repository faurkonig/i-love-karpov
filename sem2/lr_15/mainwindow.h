#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define COL_COUNT 8
#define NORMAL_FL 0x3f
#define HEADER_FL 0x3d
#define BLOCKED_FL 0x1d


#include <QMainWindow>
#include <QVector>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QAbstractButton>
// Для сохранения и загрузки файлов
#include <QFileDialog>
#include <QFile>

#include <primitive.h>
#include <paintpath.h>
#include <pixel.h>
#include <line.h>
#include <rectangle.h>
#include <ellipse.h>
#include <arc.h>
#include <curve.h>
#include <polygon.h>
// Диалоги
#include <pendialog.h>
#include <brushdialog.h>
#include <pointsdialog.h>


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

    // Слоты для полей таблицы
    void typeComboBox_clicked(int arg1);
    void penStyleButton_clicked();
    void brushStyleButton_clicked();
    void pointsDialogButton_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_pushButtonSave_clicked();

    void on_pushButtonLoad_clicked();

private:
    Ui::MainWindow *ui;

    void drawAll();

    // Действия с таблицей
    void clearItems(int index);
    void genItemsPixel(int index);
    void genItemsLine(int index);
    void genItemsRectangle(int index);
    void genItemsEllipse(int index);
    void genItemsArc(int index);
    void genItemsCurve(int index);
    void genItemsPolygon(int index);
    void genItemsFromClasses(QVector<Primitive *> * prims);

    // Методы для работы с файлами
    Pen * binGetPen(QFile &file, bool * flag = nullptr);
    Brush * binGetBrush(QFile &file, bool * flag = nullptr);
    QVector<Point> * binGetPoints(QFile &file, bool * flag = nullptr);

    QVector<Primitive *> primitives;
    QVector<QVector<bool>> incorrects;
    bool isAutoChange = false;
    QColor defaultColor;
    QColor errorColor;

    const int intSz = sizeof(int);
};
#endif // MAINWINDOW_H
