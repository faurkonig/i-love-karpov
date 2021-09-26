#include "mainwindow.h"
#include "ui_mainwindow.h"


/// Конструктор
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Простые потоки
    oneshotThread1 = new OneshotThread;
    oneshotThread2 = new OneshotThread;

    // Инициализация более сложных потоков
    initLoopThread();
    initMultiThread();
}

/// Создание и инициализация lThread
void MainWindow::initLoopThread()
{
    // Поток, запускающийся изначально
    lThread = new LoopThread(0);
    lThread->start();
    connect(lThread, &LoopThread::statusUpdate, this, &MainWindow::threadInfo);
    showLLButtons();
}
/// Создание и инициализация всех потоков multiThreads
void MainWindow::initMultiThread()
{
    // 3 параллельных потока
    for (int i = 0; i < MULTITHREAD_COUNT; i++)
    {
        multiThreads[i] = new LoopThread(i + 1);
        multiThreads[i]->start();
        connect(multiThreads[i], &LoopThread::statusUpdate, this, &MainWindow::multiThreadInfo);
    }
    showMTButtons();
}

/// Деструктор
MainWindow::~MainWindow()
{
    delete ui;
}


/// Обработчик нажатия на кнопку "Run 2 threads"
/// Запускает 2 OneshotThread-а
void MainWindow::on_pushButtonRunSimple_clicked()
{
    srand(clock());
    oneshotThread1->setCounter(rand() % 1000);
    oneshotThread1->start();
    oneshotThread2->setCounter(rand() % 1000);
    oneshotThread2->start();
    log("2 oneshot threads started");
}

/// Обработчик нажатия на кнопку "Show stats"
/// Выводит информацию о состоянии OneshotThread-ов в "консоль"
void MainWindow::on_pushButtonShowStatsSimple_clicked()
{
    log("Oneshot threads info:");
    logOneshotState(1, oneshotThread1);
    logOneshotState(2, oneshotThread2);
}

/// Вывод состояния одного OneshotThread в "консоль", номер нужен для отображения в строке
void MainWindow::logOneshotState(int number, OneshotThread *thread)
{
    int cnt = thread->getCounter();
    QString threadState = (cnt > 0)
            ? QString::number(cnt) + " msec"
            : "in work";
    log("| Oneshot thread №" + QString::number(number) + ": " + threadState);
}


/// Обработчик нажатия на кнопку "LL Work"
/// Отправляет команду "работать" lThread-у
void MainWindow::on_pushButtonLoopWork_clicked()
{
    lThread->setCommand(1);
    lThread->unlock();
    disableLLButtons();
}

/// Обработчик нажатия на кнопку "LL Kill"
/// Отправляет команду "умереть" lThread-у
void MainWindow::on_pushButtonLoopKill_clicked()
{
    lThread->setCommand(2);
    lThread->unlock();
    disableLLButtons();
}

/// Обработчик нажатия на кнопку "LL Respawn"
/// Проводит переинициализацию lThread
void MainWindow::on_pushButtonLoopRespawn_clicked()
{
    initLoopThread();
    enableLLButtons();
}

/// Слот, вызывается по сигналу statusUpdate от lThread.
/// Выводит в "консоль" его последнюю команду и время работы
void MainWindow::threadInfo()
{
    switch (lThread->getCommand()) {
    case 3: // Поток закончил работу
        lThread->setCommand(0);
        log("LL Thread stopped: " + QString::number(lThread->getCounter()) + " msec");
        enableLLButtons();
        break;
    case 4: // Поток умер
        log("LL Thread killed: " + QString::number(lThread->getCounter()) + " msec");
        hideLLButtons();
        delete lThread;
        lThread = nullptr;
        break;
    }
}

/// Блокировка кнопок блока LL на форме
void MainWindow::disableLLButtons()
{
    ui->pushButtonLoopWork->setEnabled(false);
    ui->pushButtonLoopKill->setEnabled(false);
}
/// Разблокировка кнопок блока LL на форме
void MainWindow::enableLLButtons()
{
    ui->pushButtonLoopWork->setEnabled(true);
    ui->pushButtonLoopKill->setEnabled(true);
}
/// Скрытие кнопок блока LL на форме
/// (показывает кнопку для пересоздания потока)
void MainWindow::hideLLButtons()
{
    ui->pushButtonLoopWork->hide();
    ui->pushButtonLoopKill->hide();
    ui->pushButtonLoopRespawn->show();
}
/// Отображение кнопок блока LL на форме
/// (скрывает кнопку для пересоздания потока)
void MainWindow::showLLButtons()
{
    ui->pushButtonLoopWork->show();
    ui->pushButtonLoopKill->show();
    ui->pushButtonLoopRespawn->hide();
}


/// Обработчик нажатия на кнопку "MT Work"
/// Отправляет команду "работать" multiThread
void MainWindow::on_pushButtonMultiThreadWork_clicked()
{
    setMultiThreadCommand(1);
    unlockMultiThreadMutex();
    disableMTButtons();
}

/// Обработчик нажатия на кнопку "MT Kill"
/// Отправляет команду "умереть" multiThread
void MainWindow::on_pushButtonMultiThreadKill_clicked()
{
    setMultiThreadCommand(2);
    unlockMultiThreadMutex();
    disableMTButtons();
}

/// Установка одной команды на всех потоках multiThreads
void MainWindow::setMultiThreadCommand(int command)
{
    for (int i = 0; i < MULTITHREAD_COUNT; i++)
        multiThreads[i]->setCommand(command);
}

/// Разблокивка мьютексов на всех потоках multiThreads
void MainWindow::unlockMultiThreadMutex()
{
    for (int i = 0; i < MULTITHREAD_COUNT; i++)
        multiThreads[i]->unlock();
}

/// Обработчик нажатия на кнопку "MT Respawn"
/// Проводит переинициализацию multiThread
void MainWindow::on_pushButtonMultiThreadRespawn_clicked()
{
    initMultiThread();
    enableMTButtons();
}

/// Обработчик нажатия на кнопку "MT Stats"
/// Выводит информацию обо всех потоках multiThread в "консоль"
void MainWindow::on_pushButtonMultiThreadStats_clicked()
{
    log("MultiThreads info:");
    for (int i = 0; i < MULTITHREAD_COUNT; i++)
    {
        logLoopState(multiThreads[i]);
    }
}

/// Вывод состояния одного LoopThread в "консоль"
void MainWindow::logLoopState(LoopThread *thread)
{
    QString result = "| MT thread №" + QString::number(thread->getNumber());
    switch (thread->getCommand())
    {
    case 1: // Выполняется команда "работать"
        result += " is working...";
        break;
    case 2: // Выполняется команда "умереть"
        result += " is killing itself...";
        break;
    case 3: // Поток выполнил работу
        result += " stopped in " + QString::number(thread->getCounter()) + " msec";
        break;
    case 4: // Поток умер
        result += " killed in " + QString::number(thread->getCounter()) + " msec";
        break;
    }
    log(result);
}

/// Слот, вызывается по сигналу statusUpdate от всех из multiThreads.
/// Выводит в "консоль" его последнюю команду потока и время работы (если поток закончил работу)
void MainWindow::multiThreadInfo()
{
    // Отображение информации об одном потоке (который вызвал данный сигнал)
    auto threadSender = static_cast<LoopThread *>(sender());
    if (threadSender != nullptr)
    {
        QString stateText = "MT thread №" + QString::number(threadSender->getNumber()) + ": ";
        switch (threadSender->getCommand()) {
        case 3: // Поток выполнил работу
            stateText += "stopped in ";
            break;
        case 4: // Поток умер
            stateText += "killed in ";
            break;
        }
        stateText += QString::number(threadSender->getCounter()) + " msec";
        log(stateText);
    }

    // Если все потоки выполнили команды, то выводится информация об этом
    if (isMultiThreadStopped())
    {
        // Все потоки закончили работу
        log("All " + QString::number(MULTITHREAD_COUNT) + " threads stopped");
        enableMTButtons();
    }
    else if (isMultiThreadKilled())
    {
        // Все потоки умерли
        log("All " + QString::number(MULTITHREAD_COUNT) + " threads killed");
        hideMTButtons();
        for (int i = 0; i < MULTITHREAD_COUNT; i++)
        {
            delete multiThreads[i];
            multiThreads[i] = nullptr;
        }
    }
}

/// Блокировка кнопок блока MT на форме
void MainWindow::disableMTButtons()
{
    ui->pushButtonMultiThreadWork->setEnabled(false);
    ui->pushButtonMultiThreadKill->setEnabled(false);
}
/// Разблокировка кнопок блока MT на форме
void MainWindow::enableMTButtons()
{
    ui->pushButtonMultiThreadWork->setEnabled(true);
    ui->pushButtonMultiThreadKill->setEnabled(true);
}
/// Скрытие кнопок блока MT на форме
/// (показывает кнопку для пересоздания потока)
void MainWindow::hideMTButtons()
{
    ui->pushButtonMultiThreadWork->hide();
    ui->pushButtonMultiThreadKill->hide();
    ui->pushButtonMultiThreadStats->hide();
    ui->pushButtonMultiThreadRespawn->show();
}
/// Отображение кнопок блока MT на форме
/// (скрывает кнопку для пересоздания потока)
void MainWindow::showMTButtons()
{
    ui->pushButtonMultiThreadWork->show();
    ui->pushButtonMultiThreadKill->show();
    ui->pushButtonMultiThreadStats->show();
    ui->pushButtonMultiThreadRespawn->hide();
}

/// Проверка команд multiThread. Возвращает true, если все команды - 3
bool MainWindow::isMultiThreadStopped()
{
    for (int i = 0; i < MULTITHREAD_COUNT; i++)
    {
        if (multiThreads[i]->getCommand() != 3)
            return false;
    }
    return true;
}

/// Проверка команд multiThread. Возвращает true, если все команды - 4
bool MainWindow::isMultiThreadKilled()
{
    for (int i = 0; i < MULTITHREAD_COUNT; i++)
    {
        if (multiThreads[i]->getCommand() != 4)
            return false;
    }
    return true;
}


/// Обработчик нажатия на кнопку "Clear text"
/// Очищает текст "консоли"
void MainWindow::on_pushButtonClear_clicked()
{
    ui->textEdit->clear();
}


/// Сокращение для ui->textEdit->append, выводит текст в "консоль"
void MainWindow::log(QString content)
{
    ui->textEdit->append(content);
}
