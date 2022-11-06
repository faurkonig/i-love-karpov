#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSqlDatabase>

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
    void on_connectionButton_clicked();

    void on_radioButtonUser_clicked();

    void on_radioButtonGuest_clicked();

    void on_radioButtonDeveloper_clicked();

    void on_registerPushButton_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase *mainDatabase{nullptr};
};
#endif // MAINWINDOW_H
