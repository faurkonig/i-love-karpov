#include "mainwindow.h"
#include "ui_mainwindow.h"

/// Конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->labelError->hide();
}

/// Деструктор
MainWindow::~MainWindow()
{
    delete ui;
}

/// Нажатие кнопки "Добавить"
void MainWindow::on_pushButtonAdd_clicked()
{
    QString titleText = ui->lineEditTitle->text();
    QString passwordText = ui->lineEditPassword->text();

    if (titleText.length() >= 1 && passwordText.length() >= 1)
    {
        ui->labelError->hide();

        passwords.addPassword(titleText, passwordText);

        int count = passwords.getCount();
        ui->tableWidget->setRowCount(count);

        QTableWidgetItem * twi = new QTableWidgetItem(titleText);
        ui->tableWidget->setItem(count - 1, 0, twi);
        twi = new QTableWidgetItem(passwordText);
        ui->tableWidget->setItem(count - 1, 1, twi);

        QPushButton * pb = new QPushButton("Удалить");
        pb->setProperty("row", count - 1);
        connect(pb, &QPushButton::clicked, this, &MainWindow::deleteButtonClick);
        ui->tableWidget->setCellWidget(count - 1, 2, pb);
    }
    else
    {
        // Если текст не соответствует критериям ввода, то выводим ошибку
        ui->labelError->show();
    }
}

/// Нажатие кнопки "Удалить" в таблице
void MainWindow::deleteButtonClick()
{
    QPushButton * pb = qobject_cast<QPushButton *>(sender());
    if (pb != nullptr)
    {
        int row = pb->property("row").toInt();
        passwords.removePassword(row);

        int count = passwords.getCount();
        ui->tableWidget->setRowCount(count);
        for (int i = row; i < count; i++)
        {
            QTableWidgetItem * twi = new QTableWidgetItem(passwords.getTitle(i));
            ui->tableWidget->setItem(i, 0, twi);
            twi = new QTableWidgetItem(passwords.getPass(i));
            ui->tableWidget->setItem(i, 1, twi);

            QPushButton * pb = new QPushButton("Удалить");
            pb->setProperty("row", i);
            connect(pb, &QPushButton::clicked, this, &MainWindow::deleteButtonClick);
            ui->tableWidget->removeCellWidget(i, 2);
            ui->tableWidget->setCellWidget(i, 2, pb);
        }
    }
}

/// Изменение пароля
void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
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

