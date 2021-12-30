#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "valuesarray.h"

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
    void on_pushButtonAdd_clicked();

    void on_pushButtonResult_clicked();

    void on_pushButtonClear_clicked();

private:
    Ui::MainWindow *ui;

    ValuesArray array;
};
#endif // MAINWINDOW_H
