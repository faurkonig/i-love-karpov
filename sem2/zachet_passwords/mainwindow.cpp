#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Конструктор
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    // Деструктор
    delete ui;
}

void MainWindow::on_pushButtonAdd_clicked()
{
    // Нажатие кнопки "Добавить"
    QString titleText = ui->lineEditTitle->text();
    QString passwordText = ui->lineEditPassword->text();
    passwords.addPassword(titleText, passwordText);

    int count = passwords.getCount();
    ui->tableWidget->setRowCount(count);
    QTableWidgetItem * twi = new QTableWidgetItem(titleText);
    ui->tableWidget->setItem(count - 1, 0, twi);
    twi = new QTableWidgetItem(passwordText);
    ui->tableWidget->setItem(count - 1, 1, twi);
    QPushButton * pb = new QPushButton("Удалить");
    pb->setProperty("row", count - 1);
    connect(pb, SIGNAL(clicked()), this, SLOT(deleteButtonClick()));
    ui->tableWidget->setCellWidget(count - 1, 2, pb);
}

void MainWindow::deleteButtonClick()
{
    // Нажатие кнопки "Удалить" в таблице
    QPushButton * pb = qobject_cast<QPushButton *>(sender());
    if (pb != nullptr)
    {
        int row = pb->property("row").toInt();
        passwords.removePassword(row);

        int count = passwords.getCount();
        ui->tableWidget->setRowCount(count);
        for (int i = 0; i < count; i++)
        {
            QTableWidgetItem * twi = new QTableWidgetItem(passwords.getTitle(i));
            ui->tableWidget->setItem(i, 0, twi);
            twi = new QTableWidgetItem(passwords.getPass(i));
            ui->tableWidget->setItem(i, 1, twi);
            QPushButton * pb = new QPushButton("Удалить");
            pb->setProperty("row", i);
            connect(pb, SIGNAL(clicked()), this, SLOT(deleteButtonClick()));
            ui->tableWidget->removeCellWidget(i, 2);
            ui->tableWidget->setCellWidget(i, 2, pb);
        }
    }
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    // Изменение пароля
    QString content = ui->tableWidget->item(row, column)->text();
    if (column == 0)
    {
        passwords.setTitle(row, content);
    }
    else
    {
        passwords.setPass(row, content);
    }
}

