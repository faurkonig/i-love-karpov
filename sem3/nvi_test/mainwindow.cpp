#include "mainwindow.h"
#include "ui_mainwindow.h"


/// Конструктор главного окна программы
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    testTimer = new QTimer(this);
    testTimer->setInterval(1000);
    connect(testTimer, &QTimer::timeout, this, &MainWindow::onTestTimerTimeout);

    answersButtons = new QButtonGroup(this);
    connect(answersButtons, &QButtonGroup::idClicked, this, &MainWindow::onAnswerChecked);

    updateTimeLabels(false);
    ui->resultContainer->hide();
    ui->exerciseContainer->hide();
}

/// Деструктор
MainWindow::~MainWindow()
{
    delete ui;
    clearAnswersButtons();
}


/// Обработка нажатия на кнопку "Старт"
void MainWindow::on_startButton_clicked()
{
    currentExercise = 0;
    choosedAnswers.clear();
    for (int i = 0; i < defaultTest.getExercisesCount(); i++)
        choosedAnswers.append(-1);

    ui->startButton->setEnabled(false);
    ui->resultContainer->hide();
    ui->exerciseContainer->show();

    clearAnswersButtons();
    updateExerciseWidgets();

    estimatedSeconds = defaultTest.getTimeInSeconds();
    updateTimeLabels();
    testTimer->start();
}


/// Функция, убирающая все ответы с формы
void MainWindow::clearAnswersButtons()
{
    foreach (QAbstractButton *rb, answersButtons->buttons())
    {
        ui->answersGroupBox->layout()->removeWidget(rb);
        answersButtons->removeButton(rb);
        delete rb;
    }
}

/// Функция, обновляющая виджеты задания на актуальные данные
void MainWindow::updateExerciseWidgets()
{
    auto exercise = defaultTest[currentExercise];
    ui->exerciseText->setText(QString::number(currentExercise + 1) + ". " + exercise->getText());

    auto answers = exercise->getAnswers();
    for (int i = 0; i < answers->count(); i++)
    {
        QRadioButton *rb = new QRadioButton(convertNumberToABC(i) + ") " + (*answers)[i].getText());
        answersButtons->addButton(rb);
        answersButtons->setId(rb, i);
        ui->answersGroupBox->layout()->addWidget(rb);
    }

    ui->progressBar->setMaximum(defaultTest.getExercisesCount());
    ui->progressBar->setValue(currentExercise);

    if (choosedAnswers[currentExercise] == -1)
        ui->nextExerciseButton->setEnabled(false);
    else
        ui->nextExerciseButton->setEnabled(true);
}

/// Функция, превращающая индекс ответа в латинские буквы a-z
QString MainWindow::convertNumberToABC(int number)
{
    return QString(QChar(97 + number));
}


/// Обработчик срабатывания таймера, должен вызываться раз в секунду
void MainWindow::onTestTimerTimeout()
{
    estimatedSeconds--;
    updateTimeLabels();

    if (estimatedSeconds == 0)
        finishTest();
}

/// Функция для обновления полей с текущим и оставшимся временем
/// Если в функцию передаётся true, то она ставит время на текущее,
/// если передаётся false, то она ставит время на --:--:--.
/// true по-умолчанию
void MainWindow::updateTimeLabels(bool useCurrent)
{
    if (useCurrent)
    {
        ui->estimatedTimeLabel->display(formatNumberAsTime(estimatedSeconds));
        ui->currentTimeLabel->display(formatNumberAsTime(defaultTest.getTimeInSeconds() - estimatedSeconds));
    }
    else
    {
        ui->estimatedTimeLabel->display("--:--:--");
        ui->currentTimeLabel->display("--:--:--");
    }
}

/// Функиця для форматирования количества секунд в виде hh:mm:ss
QString MainWindow::formatNumberAsTime(int seconds)
{
    if (seconds < 0)
        seconds = 0;

    int s = seconds % 60;
    int m = seconds / 60;
    int h = m / 60;
    m %= 60;

    return QString("%1:%2:%3")
            .arg(QString::number(h), 2, '0')
            .arg(QString::number(m), 2, '0')
            .arg(QString::number(s), 2, '0');
}


/// Обработчик выбора ответа на форме
void MainWindow::onAnswerChecked(int id)
{
    choosedAnswers[currentExercise] = id;

    if (currentExercise != defaultTest.getExercisesCount() - 1)
        ui->nextExerciseButton->setEnabled(true);
}


/// Обработчик нажатия на кнопку "Следующий вопрос"
void MainWindow::on_nextExerciseButton_clicked()
{
    currentExercise++;

    clearAnswersButtons();
    updateExerciseWidgets();
}


/// Обработчик нажатия на кнопку "Закончить тест"
void MainWindow::on_finishTestButton_clicked()
{
    QMessageBox::StandardButton question = QMessageBox::question(this, "Завершение",
                                                                 "Вы уверены, что хотите завершить тест?",
                                                                 QMessageBox::Yes | QMessageBox::Cancel);
    if (question == QMessageBox::Yes)
        finishTest();
}


/// Функция, которая действительно завершает тест, вызывается при конце времени и
/// при нажатии на соответствующую кнопку
void MainWindow::finishTest()
{
    testTimer->stop();

    clearAnswersButtons();
    ui->exerciseContainer->hide();
    ui->resultContainer->show();

    // ---- ОБРАБОТКА ТЕКСТА ----

    QString resultText = "<html><body>";
    resultText += "<p><b>ТЕСТ ПРОЙДЕН!</b></p>";

    int correctCount{0};
    auto exercises = defaultTest.getExercises();
    for (int i = 0; i < exercises->count(); i++)
    {
        if ((*exercises)[i].getCorrectAnswerIndex() == choosedAnswers[i])
            correctCount++;
    }

    int exerciseCount = exercises->count();
    double correctProcentDiv = static_cast<double>(correctCount) / static_cast<double>(exerciseCount);
    int correctProcent = correctProcentDiv * 100;
    resultText += QString("<p>Твой результат: %1/%2 (%3%)</p>")
            .arg(correctCount)
            .arg(exerciseCount)
            .arg(correctProcent);

    // 0-25% - неуд   (красный       | #ff0000)
    // 26-50% - удовл (оранжевый     | #ffaa00)
    // 51-75% - хор   (жёлто-зелёный | #c3ff00)
    // 76-100% - отл  (зелёный       | #00ff00)
    QString mark;
    if (correctProcent <= 25)
        mark = "<span style=\"color:#ff0000;\">неудовлетворительно</span>";
    else if (correctProcent <= 50)
        mark = "<span style=\"color:#ffaa00;\">удовлетворительно</span>";
    else if (correctProcent <= 75)
        mark = "<span style=\"color:#c3ff00;\">хорошо</span>";
    else
        mark = "<span style=\"color:#00ff00;\">отлично</span>";
    resultText += QString("<p>Оценка: %1</p>").arg(mark);

    resultText += "</body></html>";

    ui->resultLabel->setText(resultText);

    // ---- ОБРАБОТКА ТАБЛИЦЫ ----

    ui->tableWidget->setRowCount(exercises->count());
    for (int i = 0; i < exercises->count(); i++)
    {
        QTableWidgetItem *twi{nullptr};

        QString chAnswer;
        if (choosedAnswers[i] != -1)
            chAnswer = convertNumberToABC(choosedAnswers[i]);
        else
            chAnswer = "Не выбрано";
        twi = new QTableWidgetItem(chAnswer);
        twi->setFlags(twi->flags() & 0xffd);
        ui->tableWidget->setItem(i, 0, twi);

        int correctAnswerIndex = defaultTest[i]->getCorrectAnswerIndex();
        twi = new QTableWidgetItem(convertNumberToABC(correctAnswerIndex));
        twi->setFlags(twi->flags() & 0xffd);
        ui->tableWidget->setItem(i, 1, twi);

        twi = new QTableWidgetItem(choosedAnswers[i] == correctAnswerIndex ? "1" : "0");
        twi->setFlags(twi->flags() & 0xffd);
        ui->tableWidget->setItem(i, 2, twi);
    }

    ui->startButton->setEnabled(true);
}


/// Обработчик нажатия на кнопку "Закрыть программу"
void MainWindow::on_closeButton_clicked()
{
    close();
}
