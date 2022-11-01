#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

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
    void on_pushButtonConnect_clicked();

    void on_pushButtonTestDriver_clicked();

    void on_pushButtonSimpleOpen_clicked();

    void on_pushButtonReadDB_clicked();

    void on_pushButtonInsert_clicked();

    void on_pushButtonReadDB2_clicked();

private:
    Ui::MainWindow *ui;
    QSqlDatabase *db;
    QSqlDatabase db1;
};

#endif // MAINWINDOW_H
