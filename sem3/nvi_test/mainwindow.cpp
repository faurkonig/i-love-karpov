#include "mainwindow.h"
#include "ui_mainwindow.h"

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

MainWindow::~MainWindow()
{
    delete ui;
    clearAnswersButtons();
}


void MainWindow::on_startButton_clicked()
{
    currentExercise = 0;
    correctCount = 0;
    currentAnswer = -1;

    ui->startButton->setEnabled(false);
    ui->exerciseContainer->show();

    clearAnswersButtons();
    updateExerciseWidgets();

    estimatedSeconds = defaultTest.getTimeInSeconds();
    updateTimeLabels();
    testTimer->start();
}


void MainWindow::clearAnswersButtons()
{
    foreach (QAbstractButton *rb, answersButtons->buttons())
    {
        ui->answersGroupBox->layout()->removeWidget(rb);
        answersButtons->removeButton(rb);
        delete rb;
    }
}

void MainWindow::updateExerciseWidgets()
{
    auto exercise = defaultTest[currentExercise];
    ui->exerciseText->setText(QString::number(currentExercise + 1) + ". " + exercise->getText());

    auto answers = exercise->getAnswers();
    for (int i = 0; i < answers->count(); i++)
    {
        QChar exerciseSymbol = QChar(97 + i);
        QRadioButton *rb = new QRadioButton(QString(exerciseSymbol) + ") " + (*answers)[i].getText());
        answersButtons->addButton(rb);
        answersButtons->setId(rb, i);
        ui->answersGroupBox->layout()->addWidget(rb);
    }

    ui->progressBar->setMaximum(defaultTest.getExercises()->count());
    ui->progressBar->setValue(currentExercise);

    if (currentAnswer == -1)
        ui->nextExerciseButton->setEnabled(false);
    else
        ui->nextExerciseButton->setEnabled(true);
}


void MainWindow::onTestTimerTimeout()
{
    estimatedSeconds--;
    updateTimeLabels();

    if (estimatedSeconds == 0)
        finishTest();
}


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


void MainWindow::onAnswerChecked(int id)
{
    currentAnswer = id;

    if (currentExercise != defaultTest.getExercises()->count() - 1)
        ui->nextExerciseButton->setEnabled(true);
}


void MainWindow::on_nextExerciseButton_clicked()
{
    if (defaultTest[currentExercise]->getCorrectAnswerIndex() == currentAnswer)
        correctCount++;

    currentAnswer = -1;
    currentExercise++;

    clearAnswersButtons();
    updateExerciseWidgets();
}


void MainWindow::on_finishTestButton_clicked()
{
    QMessageBox::StandardButton question = QMessageBox::question(this, "Завершение",
                                                                 "Вы уверены, что хотите завершить тест?",
                                                                 QMessageBox::Yes | QMessageBox::Cancel);
    if (question == QMessageBox::Yes)
        finishTest();
}


void MainWindow::finishTest()
{
    if (defaultTest[currentExercise]->getCorrectAnswerIndex() == currentAnswer)
        correctCount++;

    testTimer->stop();

    clearAnswersButtons();
    ui->exerciseContainer->hide();
    ui->resultContainer->show();

    QString resultText = "<html><body>";
    resultText += "<p><b>ТЕСТ ПРОЙДЕН!</b></p>";

    int exerciseCount = defaultTest.getExercises()->count();
    double correctProcentDiv = static_cast<double>(correctCount) / static_cast<double>(exerciseCount);
    int correctProcent = correctProcentDiv * 100;
    resultText += QString("<p>Твой результат: %1/%2 (%3%)</p>")
            .arg(correctCount)
            .arg(exerciseCount)
            .arg(correctProcent);

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

    ui->startButton->setEnabled(true);
}


void MainWindow::on_closeButton_clicked()
{
    close();
}
