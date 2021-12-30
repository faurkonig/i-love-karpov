#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtNetwork/QNetworkDatagram>
#include <QtNetwork/QNetworkInterface>
#include <QValidator>

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

    if (our_port > 0 && our_port < 0x10000 && port > 0 && port < 0x10000)
    {
        QString textAddress = ui->lineEdit_Adress->text();
        // Регулярное выражение, проверяющее строку на соответствие формату IPv4
        QRegExp ipv4RegEx("^((25[0-5]|(2[0-4]|1\\d|[1-9]|)\\d)(\\.(?!$)|$)){4}$");
        if (ipv4RegEx.indexIn(textAddress) != -1) // Если нашлось совпадение
        {
            if (udpsocket != nullptr)
            {
                udpsocket->close();
                udpsocket->deleteLater();
                udpsocket = nullptr;
            }

            udpsocket = new QUdpSocket(this);
            if (udpsocket->bind(QHostAddress::Any, quint16(our_port)))
            {
                connect(udpsocket, &QUdpSocket::readyRead, this, &MainWindow::read_data);
                if (address != nullptr)
                    delete address;
                address = new QHostAddress();
                address->setAddress(textAddress);

                QMessageBox::warning(this, "Сообщение", "Сокет создан и принимает сообщения на порт "+ui->lineEdit_Our_Port->text());

                ui->lineEdit_Mes->setEnabled(true);
                ui->pushButton_SendMes->setEnabled(true);
            }
            else
            {
                QMessageBox::warning(this, "Ошибка", "Не удалось открыть порт");
            }
        }
        else
        {
            QMessageBox::warning(this, "Ошибка", "Введён неверный адрес");
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Введён неверный порт");
    }
}

void MainWindow::on_pushButton_SendMes_clicked()
{
    QString str = ui->lineEdit_Mes->text();
    QByteArray ba = str.toLocal8Bit(); // Переводим строку в массив байт
    if (ba.size() > 0)
    {
        udpsocket->writeDatagram(ba, *address, quint16(port)); // Отправляем данные
        ui->textEdit_Output->append(str);

        // Очищаем поле ввода и ставим фокус на него
        ui->lineEdit_Mes->clear();
        ui->lineEdit_Mes->setFocus();
    }
}

void MainWindow::spam_timeout()
{
    if (ui->checkBoxSpam->isChecked() && ui->lineEdit_Mes->isEnabled())
    {
        QString str = "Привет!";
        QByteArray ba = str.toLocal8Bit(); // Переводим строку в массив байт
        udpsocket->writeDatagram(ba, *address, quint16(port)); // Отправляем данные
        ui->textEdit_Output->append(str);
    }
}

void MainWindow::on_pushButton_Address_clicked()
{    
    QList<QHostAddress> adrs = QNetworkInterface::allAddresses(); // Получаем все адреса
    ui->textEdit_Address->clear();
    ui->textEdit_Address->append("IP адреса компьютера:");
    for (int i = 0; i < adrs.count(); i++)
    {
        if (adrs[i].protocol() == QAbstractSocket::IPv4Protocol)
        {
            QString str = adrs[i].toString().split(':').last();
            ui->textEdit_Address->append(str);
        }
    }
}
