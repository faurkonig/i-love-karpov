#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define COL_COUNT 8
#define NORMAL_FL 0x3f
#define HEADER_FL 0x3d
#define BLOCKED_FL 0x1d
#define DEF_FILE_DIR "/home/vlad/Documents/Projects/QT/_smth"

#include <QMainWindow>
#include <QVector>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QPushButton>
#include <QMessageBox>
#include <QAbstractButton>
// Для сохранения и загрузки файлов
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
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

    // Слоты, сгенерированные Qt
    void on_tableWidget_cellChanged(int row, int column);
    void on_action_JSON_save_triggered();
    void on_action_BIN_save_triggered();
    void on_action_JSON_open_triggered();
    void on_action_BIN_open_triggered();

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

    // Действия с JSON
    int jsonGetNum(QJsonValueRef val, bool * flag = nullptr);
    QString jsonGetString(QJsonValueRef val, bool * flag = nullptr);
    Color jsonGetColor(QJsonValueRef val, bool * flag = nullptr);
    Pen * jsonGetPen(QJsonValueRef val, bool * flag = nullptr);
    Brush * jsonGetBrush(QJsonValueRef val, bool * flag = nullptr);
    Point jsonGetPoint(QJsonValueRef val, bool * flag = nullptr);
    QVector<Point> * jsonGetPoints(QJsonValueRef val, bool * flag = nullptr);
    Pixel * jsonGetPixel(QJsonValueRef val);
    Line * jsonGetLine(QJsonObject val);
    Rectangle * jsonGetRectangle(QJsonObject val);
    Ellipse * jsonGetEllipse(QJsonValueRef val);
    Arc * jsonGetArc(QJsonValueRef val);

    // Действия с бинарниками
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
