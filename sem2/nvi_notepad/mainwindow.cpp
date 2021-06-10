#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MainWindow::setWindowTitle("Блокнот - Новый файл");

    // декодер для чтения текста
    decoder = new QTextDecoder(codec);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Menu_Create_triggered()
{
    if (textChanged)
    {
        // если текст изменён
        // вызов вопроса о сохранении
        QMessageBox::StandardButton question = QMessageBox::question(this, "Выход", "Сохранить файл?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
        if (question == QMessageBox::Yes)
        {
            // если надо сохранить
            if (saveHandler())
            {
                // если сохранение прошло успешно
                // очистка поля
                ui->textEdit->clear();

                textChanged = false;
                MainWindow::setWindowTitle("Блокнот - Новый файл");
            }
        }
        else if (question == QMessageBox::No)
        {
            // если не надо сохранять
            // очистка поля
            ui->textEdit->clear();

            textChanged = false;
            MainWindow::setWindowTitle("Блокнот - Новый файл");
        }
    }
    else
    {
        // если текст не изменён
        // очистка поля
        ui->textEdit->clear();

        textChanged = false;
        MainWindow::setWindowTitle("Блокнот - Новый файл");
    }
}

void MainWindow::on_Menu_Open_triggered()
{
    if (textChanged)
    {
        // если текст изменён
        // вызов вопроса о сохранении
        QMessageBox::StandardButton question = QMessageBox::question(this, "Выход", "Сохранить файл?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
        if (question == QMessageBox::Yes)
        {
            // если надо сохранить
            if (saveHandler())
            {
                // если сохранение прошло успешно
                openHandler();
            }
        }
        else if (question == QMessageBox::No)
        {
            // если не надо сохранять
            openHandler();
        }
    }
    else
    {
        // если текст не изменён
        openHandler();
    }
}

void MainWindow::on_Menu_Save_triggered()
{
    saveHandler();
}

void MainWindow::on_Menu_SaveAs_triggered()
{
    QString fileName; // имя файла

    // выбор файла
    fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "C:/Users/student/Documents/nvi_notepad_dir", tr("Text files (*.txt)"));

    if (fileName.isEmpty()) // если файл не выбран
    {
        // вывод предупреждения
        QMessageBox::information(this, "Ошибка", "Файл на выбран!");
    }
    else
    {
        QFile file; // объявляем объект файла
        file.setFileName(fileName); // связываем с файлом
        file.open(QIODevice::WriteOnly); // открываем файл

        // записываем данные в файл
        file.write(ui->textEdit->toPlainText().toLocal8Bit());

        file.close(); // закрываем файл

        currentFileName = fileName;
        MainWindow::setWindowTitle("Блокнот - " + currentFileName);
        textChanged = false;
    }
}

void MainWindow::on_Menu_Quit_triggered()
{
    // выход из программы
    MainWindow::close();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (textChanged)
    {
        // если текст изменён
        // вызов вопроса о сохранении
        QMessageBox::StandardButton question = QMessageBox::question(this, "Выход", "Сохранить файл?", QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
        if (question == QMessageBox::Yes)
        {
            // если надо сохранить
            if (saveHandler())
            {
                // если сохранение прошло успешно
                // принимаем выход
                event->accept();
            }
            else
            {
                // если сохранение прошло не успешно
                // не выходим
                event->ignore();
            }
        }
        else if (question == QMessageBox::No)
        {
            // если не надо сохранять
            // принимаем выход
            event->accept();
        }
        else
        {
            // если "отмена"
            // не выходим
            event->ignore();
        }
    }
    else
    {
        // если текст не изменён
        event->accept();
    }
}

void MainWindow::on_textEdit_textChanged()
{
    textChanged = true;
}

// функция для сохранения файла
bool MainWindow::saveHandler()
{
    if (currentFileName.isEmpty())
    {
        // выбор файла
        QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "C:/Users/student/Documents/nvi_notepad_dir", tr("Text files (*.txt)"));

        if (fileName.isEmpty()) // если файл не выбран
        {
            // вывод предупреждения
            QMessageBox::information(this, "Ошибка", "Файл на выбран!");

            return false;
        }
        else
        {
            QFile file; // объявляем объект файла
            file.setFileName(fileName); // связываем с файлом
            file.open(QIODevice::WriteOnly); // открываем файл

            // записываем данные в файл
            file.write(ui->textEdit->toPlainText().toLocal8Bit());

            file.close(); // закрываем файл

            currentFileName = fileName;
            MainWindow::setWindowTitle("Блокнот - " + currentFileName);

            textChanged = false;

            return true;
        }
    }
    else
    {
        QFile file; // объявляем объект файла
        file.setFileName(currentFileName); // связываем с файлом
        file.open(QIODevice::WriteOnly); // открываем файл

        // записываем данные в файл
        file.write(ui->textEdit->toPlainText().toLocal8Bit());

        file.close(); // закрываем файл

        textChanged = false;

        return true;
    }
}

// функция для открытия выбора файла
void MainWindow::openHandler()
{
    QString fileName; // имя файла

    // выбор файла
    fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "C:/Users/student/Documents/nvi_notepad_dir", tr("Text files (*.txt)"));

    if (fileName.isEmpty()) // если файл не выбран
    {
        // вывод предупреждения
        QMessageBox::information(this, "Ошибка", "Файл на выбран!");
    }
    else
    {
        ui->textEdit->clear();
        QFile file; // объявляем объект файла
        file.setFileName(fileName); // связываем с файлом
        file.open(QIODevice::ReadOnly); // открываем файл

        QByteArray ba; // массив байтов для передачи файлов
        ba.clear();

        qint64 size = file.size();
        // считываем данные из файла
        ba = file.read(size);
        ui->textEdit->append(decoder->toUnicode(ba));

        file.close(); // закрываем файл

        currentFileName = fileName;
        MainWindow::setWindowTitle("Блокнот - " + currentFileName);

        textChanged = false;
    }
}
