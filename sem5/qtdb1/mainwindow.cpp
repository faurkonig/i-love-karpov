#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = nullptr;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonConnect_clicked()
{
    if (db != nullptr)
    {
        db->close();
        delete db;
        db = nullptr;
        return;
    }
    else
    {
        db = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
    }

    QString dbName{"MY_DB"};
    QString host{"127.0.0.1"};
    QString usr{"postgres"};
    QString pwd{"1234"};

    db->setDatabaseName(dbName);
    db->setHostName(host);
    db->setUserName(usr);
    db->setPassword(pwd);
    db->setPort(5432);

    if (db->isValid())
    {
        QMessageBox::information(this, "Valid", "Yes");
    }
    else
    {
        QMessageBox::information(this, "Valid", "No");
    }

    if (db->isDriverAvailable("QPSQL"))
    {
        QMessageBox::information(this, "Driver available", "Yes");
    }
    else
    {
        QMessageBox::information(this, "Driver available", "No");
    }

    if (!db->open())
    {
        QMessageBox::warning(this, "Error", db->lastError().text());
        delete db;
        db = nullptr;
    }
}

void MainWindow::on_pushButtonTestDriver_clicked()
{
    QStringList driverList(QSqlDatabase::drivers());
    ui->textEdit->append(driverList.join(", "));
}

void MainWindow::on_pushButtonSimpleOpen_clicked()
{
    if (db != nullptr)
    {
        db->close();
        delete db;
        db = nullptr;
        return;
    }
    else
    {
        db = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
    }

    QString dbName{"MY_DB"};
    QString host{"127.0.0.1"};
    QString usr{"postgres"};
    QString pwd{"1234"};

    db->setDatabaseName(dbName);
    db->setHostName(host);
    db->setUserName(usr);
    db->setPassword(pwd);
    db->setPort(5432);

    if (!db->open())
    {
        QMessageBox::warning(this, "Error", db->lastError().text());
        delete db;
        db = nullptr;
    }
    else
    {
        QMessageBox::information(this, "Ok", "Ok");
    }
}

void MainWindow::on_pushButtonReadDB_clicked()
{
    if (db != nullptr && db->isOpen())
    {
        QSqlQuery qw;
        qw = db->exec("SELECT * FROM \"test\";");
        // QMessageBox::warning(this, "Error", db->lastError().text());
        while (qw.next()) {
            ui->textEdit->append("Data: " + qw.value(0).toString() + " " + qw.value(1).toString());
        }
    }
    else
    {
        QMessageBox::information(this, "Error", "DB is not opened");
    }
}

void MainWindow::on_pushButtonInsert_clicked()
{
    if (db != nullptr && db->isOpen())
    {
        QSqlQuery qw;
        QString query{"INSERT INTO \"test\" VALUES ("};
        query.append(QString::number(4) + ", ");
        query.append("'BlaBla');");
        qw = db->exec(query);
        // QMessageBox::warning(this, "Error", db->lastError().text());
    }
    else
    {
        QMessageBox::information(this, "Error", "DB is not opened");
    }
}

void MainWindow::on_pushButtonReadDB2_clicked()
{

    if (db != nullptr && db->isOpen())
    {
        QSqlQuery qw;
        qw = db->exec("SELECT count(id) FROM \"test\";");
        // QMessageBox::warning(this, "Error", db->lastError().text());
        qw.first();
        int row = qw.value(0).toInt();
        ui->tableWidget->setRowCount(row);
        qw = db->exec("SELECT * FROM \"test\";");
        int currentRow = 0;
        while (qw.next()) {
            if (ui->tableWidget->item(currentRow, 0) == nullptr)
            {
                QTableWidgetItem *ti = new QTableWidgetItem;
                ui->tableWidget->setItem(currentRow, 0, ti);
            }
            if (ui->tableWidget->item(currentRow, 1) == nullptr)
            {
                QTableWidgetItem *ti = new QTableWidgetItem;
                ui->tableWidget->setItem(currentRow, 1, ti);
            }
            ui->tableWidget->item(currentRow, 0)->setText(qw.value(0).toString());
            ui->tableWidget->item(currentRow, 1)->setText(qw.value(1).toString());
            currentRow++;
        }
    }
    else
    {
        QMessageBox::information(this, "Error", "DB is not opened");
    }
}
