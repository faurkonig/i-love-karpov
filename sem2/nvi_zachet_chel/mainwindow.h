#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <human.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonDraw_clicked();

private:
    Ui::MainWindow *ui;

    Human man;
};

#endif // MAINWINDOW_H
