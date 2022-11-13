#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHash>

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
        QString query{"INSERT INTO \"test\" (name) VALUES ('BlaBla');"};
        qw = db->exec(query);
        QMessageBox::warning(this, "Error", qw.lastError().text());
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
        // Количество
        auto countQuery = db->exec("SELECT count(id) FROM books;");
        countQuery.first();
        int row = countQuery.value(0).toInt();
        ui->tableWidget->setRowCount(row);

        // Сами данные
        auto booksQuery = db->exec("SELECT \"name\", provider, age FROM books;");
        /*
        name                    |provider|age|
        ------------------------+--------+---+
        Остров сокровищ         |       1|  2|
        1984                    |       2|  3|
        Преступление и наказание|       1|  3|
        */
        int currentRow = 0;
        QSqlQuery providerQuery;
        QSqlQuery ageQuery;

        QHash<QString, QString> providers;
        // 1 = Эксмо
        // 2 = Феникс

        while (booksQuery.next()) {
            initItems(ui->tableWidget, currentRow);

            // Записыванию имя книги
            ui->tableWidget->item(currentRow, 0)->setText(booksQuery.value(0).toString());

            // Запрашиваю поставщика
            auto providerId = booksQuery.value(1).toString();
            QString provider;
            if (!providers.contains(providerId)) {
                // Если нет в словаре
                providerQuery = db->exec("SELECT \"name\" FROM providers WHERE id = " + providerId);
                /*
                name |
                -----+
                Эксмо|
                */
                providerQuery.first();
                provider = providerQuery.value(0).toString();

                QMessageBox::information(this, "Запрос", "Ответ: " + provider);

                providers[providerId] = provider; // Сохраняем в словарь
            } else {
                // Если есть в словаре
                provider = providers[providerId]; // Берём из словаря
            }
            // Записываю имя поставщика
            ui->tableWidget->item(currentRow, 1)->setText(provider);

            // Запрашиваю возраст
            ageQuery = db->exec("SELECT age FROM ages WHERE id = " + booksQuery.value(2).toString());
            ageQuery.first();
            // Записываю возрастное ограничение книги
            ui->tableWidget->item(currentRow, 2)->setText(ageQuery.value(0).toString() + "+");

            currentRow++;
        }
    }
    else
    {
        QMessageBox::information(this, "Error", "DB is not opened");
    }
}

void MainWindow::initItems(QTableWidget *table, int index) {
    for (auto i = 0; i < table->columnCount(); i++) {
        if (table->item(index, i) == nullptr) {
            QTableWidgetItem *ti = new QTableWidgetItem;
            table->setItem(index, i, ti);
        }
    }
}
