#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connectiondialog.h"
#include "registerdialog.h"
#include "storewindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      DatabaseContainer(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectionButton_clicked()
{
    auto newConnection = new QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
    auto isSaved = new bool(false);
    auto connectionDialog = ConnectionDialog(newConnection, isSaved, this);
    connectionDialog.exec();

    if (*isSaved) {
        if (mainDatabase != nullptr) {
            if (mainDatabase->isOpen()) mainDatabase->close();
            delete mainDatabase;
        }

        mainDatabase = newConnection;

        ui->signInBlock->setEnabled(true);
    }

    delete isSaved;
}


void MainWindow::on_radioButtonGuest_clicked()
{
    ui->formFrame->setEnabled(false);
}

void MainWindow::on_radioButtonUser_clicked()
{
    ui->formFrame->setEnabled(true);
    ui->loginLabel->setText("Логин:");
}

void MainWindow::on_radioButtonDeveloper_clicked()
{
    ui->formFrame->setEnabled(true);
    ui->loginLabel->setText("Почта:");
}


void MainWindow::on_loginPushButton_clicked()
{
    if (ui->radioButtonGuest->isChecked()) {
        auto window = new StoreWindow(SgUser(0, "_guest", "", "Гость", QDateTime::currentDateTime()), mainDatabase);
        window->show();
    }
}

void MainWindow::on_registerPushButton_clicked()
{
    auto registerDialog = RegisterDialog(mainDatabase, this);
    registerDialog.exec();
}
