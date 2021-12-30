#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDebug>

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
    void on_clearButton_clicked();

    void on_createButton_clicked();

    void on_nameValue_textChanged(const QString &arg1);

    void on_streightValue_textChanged(const QString &arg1);

    void on_dexterityValue_textChanged(const QString &arg1);

    void on_intelligenceValue_textChanged(const QString &arg1);

    void on_luckValue_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    int points;
};

#endif // MAINWINDOW_H
