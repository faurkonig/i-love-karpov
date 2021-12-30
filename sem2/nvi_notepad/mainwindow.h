#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTextDecoder>
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
    void on_Menu_Open_triggered();

    void on_Menu_SaveAs_triggered();

    void closeEvent(QCloseEvent *event); // обработчик закрытия

    void on_Menu_Quit_triggered();

    void on_Menu_Create_triggered();

    void on_Menu_Save_triggered();

    void on_textEdit_textChanged();

private:
    Ui::MainWindow *ui;

    QString currentFileName;

    bool textChanged = false;

    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    QTextDecoder *decoder;

    bool saveHandler();
    void openHandler();
};

#endif // MAINWINDOW_H
