#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QNetworkDatagram>
#include <QtNetwork/QNetworkInterface>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->lineEdit_Mes->setEnabled(false);
    ui->pushButton_SendMes->setEnabled(false);

    // Соединяем сигнал нажатия на Enter в поле ввода со слотом отправки сообщения
    connect(ui->lineEdit_Mes, &QLineEdit::returnPressed, this, &MainWindow::on_pushButton_SendMes_clicked);

    // Ставим интервал таймера на 1 секунду, соединяем слот, запускаем таймер
    spamTimer.setInterval(1000);
    connect(&spamTimer, &QTimer::timeout, this, &MainWindow::spam_timeout);
    spamTimer.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::read_data()
{
    auto data = udpsocket->receiveDatagram().data(); // Считываем данные
    if (data.size() > 0)
    {
        QString stroka;
        stroka = QString::fromLocal8Bit(data); // Превращаем массив байт в строку
        ui->textEdit_Input->append(stroka);
    }
}


void MainWindow::on_pushButton_Create_clicked()
{
    // Считываем порты
    port = ui->lineEdit_Port->text().toInt();
    our_port = ui->lineEdit_Our_Port->text().toInt();

    if (our_port != 0)
    {
        if (udpsocket != nullptr)
        {
            udpsocket->close();
            udpsocket->deleteLater();
            udpsocket = nullptr;
        }

        udpsocket = new QUdpSocket(this);
        udpsocket->bind(QHostAddress::Any,quint16(our_port));
        connect(udpsocket, &QUdpSocket::readyRead, this, &MainWindow::read_data);
        if (address != nullptr) address = nullptr;
        address = new QHostAddress();
        address->setAddress(ui->lineEdit_Adress->text());

        QMessageBox::warning(this, "Сообщение", "Сокет создан и принимает сообщения на порт "+ui->lineEdit_Our_Port->text());

        ui->lineEdit_Mes->setEnabled(true);
        ui->pushButton_SendMes->setEnabled(true);
    }
    else
    {
        QMessageBox::warning(this, "Сообщение", "Введён неверный порт");
    }
}

void MainWindow::on_pushButton_SendMes_clicked()
{
    QByteArray ba = ui->lineEdit_Mes->text().toLocal8Bit(); // Переводим строку в массив байт
    if (ba.size() > 0)
    {
        udpsocket->writeDatagram(ba, *address, quint16(port)); // Отправляем данные
        ui->textEdit_Output->append(ui->lineEdit_Mes->text());

        // Очищаем поле ввода и ставим фокус на него
        ui->lineEdit_Mes->clear();
        ui->lineEdit_Mes->setFocus();
    }
}

void MainWindow::spam_timeout()
{
    if (ui->checkBoxSpam->isChecked() && ui->lineEdit_Mes->isEnabled())
    {
        QString str = QString("Привет!");
        QByteArray ba = str.toLocal8Bit(); // Переводим строку в массив байт
        udpsocket->writeDatagram(ba, *address, quint16(port)); // Отправляем данные
        ui->textEdit_Output->append(str);
    }
}

void MainWindow::on_pushButton_Address_clicked()
{

        auto adrs = QNetworkInterface::allAddresses(); // Получаем все адреса
        QRegExp ipv4AddressRegex("^((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)(\\.(?!$)|$)){4}$"); // Регулярное выражение IPv4 адреса
        ui->textEdit_Address->append("IP адреса компьютера:");
        for (int i = 0; i < adrs.count(); i++)
        {
            // Отсеиваем только адреса типа IPv4
            auto str = adrs[i].toString().split(':').last();
            if (ipv4AddressRegex.indexIn(str) != -1)
                ui->textEdit_Address->append(str);
        }
}
