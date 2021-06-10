#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>
#include <QLabel>

namespace Ui {
class MyWindow;
}

class MyWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MyWindow(QWidget *parent = nullptr, QLabel * l = nullptr);
    ~MyWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::MyWindow *ui;
    QLabel * label;
};

#endif // MYWINDOW_H
