#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCheckBox>

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
    QString title = ui->lineEditText->text();

    if (title.length() > 0)
    {
        ui->labelError->hide();

        tasks.addTask(title);

        int count = tasks.getCount();
        ui->tableWidget->setRowCount(count);

        QTableWidgetItem * twi = new QTableWidgetItem(title);
        twi->setFlags(twi->flags() & 0xfd);
        ui->tableWidget->setItem(count - 1, 0, twi);

        QCheckBox *cb = new QCheckBox();
        cb->setProperty("row", count - 1);
        connect(cb, &QCheckBox::toggled, this, &MainWindow::setCompletedClick);
        ui->tableWidget->setCellWidget(count - 1, 1, cb);


        QPushButton * pb = new QPushButton("Удалить");
        pb->setProperty("row", count - 1);
        connect(pb, &QPushButton::clicked, this, &MainWindow::deleteButtonClick);
        ui->tableWidget->setCellWidget(count - 1, 2, pb);
    }
    else
    {
        ui->labelError->show();
    }
}

/// Переключение чекбокса на задании
void MainWindow::setCompletedClick(bool value)
{
    QCheckBox *cb = qobject_cast<QCheckBox *>(sender());
    if (cb != nullptr)
    {
        int row = cb->property("row").toInt();
        tasks.setCompleted(row, value);
    }
}

/// Удаление задания и таблицы
void MainWindow::deleteButtonClick()
{
    QPushButton *pb = qobject_cast<QPushButton *>(sender());
    if (pb != nullptr)
    {
        int row = pb->property("row").toInt();
        tasks.removeTask(row);

        int count = tasks.getCount();
        ui->tableWidget->setRowCount(count);
        for (int i = row; i < count; i++)
        {
            QTableWidgetItem * twi = new QTableWidgetItem(tasks.getTitle(i));
            twi->setFlags(twi->flags() & 0xfd);
            ui->tableWidget->setItem(i, 0, twi);

            QCheckBox *cb = new QCheckBox();
            cb->setProperty("row", i);
            connect(cb, &QCheckBox::toggled, this, &MainWindow::setCompletedClick);
            cb->setChecked(tasks.getCompleted(i));
            ui->tableWidget->setCellWidget(i, 1, cb);

            QPushButton * pb = new QPushButton("Удалить");
            pb->setProperty("row", i);
            connect(pb, &QPushButton::clicked, this, &MainWindow::deleteButtonClick);
            ui->tableWidget->setCellWidget(i, 2, pb);
        }
    }
}
