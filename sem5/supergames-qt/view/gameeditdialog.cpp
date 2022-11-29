#include "gameeditdialog.h"
#include "ui_gameeditdialog.h"
#include "utils/dialoghelper.h"
#include <QMessageBox>
#include <QtMath>

GameEditDialog::GameEditDialog(bool isNew, int gameId, int devId, bool *isChanged, QSqlDatabase *newDb, QWidget *parent) :
    QDialog(parent),
    DatabaseContainer(this, newDb),
    ui(new Ui::GameEditDialog),
    isGameNew(isNew),
    gameId(gameId),
    devId(devId),
    isSomethingChanged(isChanged)
{
    ui->setupUi(this);

    *isSomethingChanged = false;
    updateContent();
}

GameEditDialog::~GameEditDialog()
{
    delete ui;
}


void GameEditDialog::updateContent()
{
    if (isGameNew) {
        // Обновление под добавление новой игры
        ui->titleLabel->setText("Добавление игры");
        ui->deleteButton->hide();
        ui->nameField->clear();
        ui->descriptionField->clear();
        ui->priceSpinBox->setValue(0);

        // Сброс сохранённых значений
        lastSavedName.clear();
        lastSavedDescription.clear();
        lastSavedPrice = 0;
    } else {
        // Обновление под редактирование уже существующей игры
        ui->titleLabel->setText("Редактирование игры");
        ui->deleteButton->show();

        bool ok;
        // Получение данных об игре из БД
        auto gameQ = execQuery(QString("SELECT \"name\", description, price "
                                       "FROM public.games "
                                       "WHERE id = %1").arg(gameId), ok);
        if (!ok || gameQ.size() < 1) return;
        gameQ.first();
        lastSavedName = gameQ.value(0).toString();
        lastSavedDescription = gameQ.value(1).toString();
        lastSavedPrice = gameQ.value(2).toDouble();

        // Обновление полей на форме
        ui->nameField->setText(lastSavedName);
        ui->descriptionField->setText(lastSavedDescription);
        ui->priceSpinBox->setValue(lastSavedPrice);
    }
}


void GameEditDialog::on_saveButton_clicked()
{
    // Получаем значения с формы
    auto name = ui->nameField->text().trimmed();
    auto description = ui->descriptionField->toPlainText();
    auto price = ui->priceSpinBox->value();

    if (name.isEmpty()) {
        DialogHelper::showValidationError(this, "Имя игры не может быть пустым");
        return;
    }

    bool ok;
    if (isGameNew) {
        auto newGameQ = execQuery(QString("INSERT INTO public.games "
                                          "(\"name\", description, developer, price) "
                                          "VALUES ('%2', '%3', %1, %4) "
                                          "RETURNING id").arg(devId)
                                  .arg(name.replace("'", "''"), description.replace("'", "''"))
                                  .arg(price), ok);
        if (!ok || newGameQ.size() < 1) return;
        // Получаем ID новой игры
        newGameQ.first();
        gameId = newGameQ.value(0).toInt();
        // Отмечаем игру, как уже существующую
        isGameNew = false;

        QMessageBox::information(this, "Успех",
                                 QString("Игра \"%1\" успешно добавлена").arg(name));
    } else {
        // Обновляем игру в БД
        execQuery(QString("UPDATE public.games "
                          "SET \"name\" = '%2', description = '%3', price = %4 "
                          "WHERE id = %1").arg(gameId)
                  .arg(name.replace("'", "''"), description.replace("'", "''"))
                  .arg(price), ok);
        if (!ok) return;

        QMessageBox::information(this, "Успех", "Игра успешно отредактированна");
    }

    *isSomethingChanged = true;
    updateContent();
}

void GameEditDialog::on_deleteButton_clicked()
{
    // Переспрашиваем пользователя
    auto question = QMessageBox::question(this, "Игра",
                                          QString("Вы уверены, что хотите удалить игру \"%1\"?")
                                          .arg(lastSavedName),
                                          QMessageBox::Yes | QMessageBox::No);
    if (question != QMessageBox::Yes) return;

    // Предохранение на случай, если мы хотим удалить несуществующую игру
    if (isGameNew) return;

    bool ok;
    // Удаление игры
    execQuery(QString("DELETE FROM public.games "
                      "WHERE id = %1").arg(gameId), ok);
    if (!ok) return;

    QMessageBox::information(this, "Успех", "Игра удалена :(");

    isGameNew = true;
    *isSomethingChanged = true;
//    updateContent();
    accept();
}

void GameEditDialog::on_cancelButton_clicked()
{
    reject();
}


void GameEditDialog::reject()
{
    // Получаем значения с формы
    auto currentName = ui->nameField->text().trimmed();
    auto currentDescription = ui->descriptionField->toPlainText();
    auto currentPrice = ui->priceSpinBox->value();

    if (lastSavedName != currentName ||
            lastSavedDescription != currentDescription ||
            qAbs(lastSavedPrice - currentPrice) >= 0.01) {
        // Если были выполнены какие-либо изменения
        // относительно последнего сохранённого состояния
        auto question = QMessageBox::question(this, "Игра",
                                              "Содержатся несохранённые изменения. "
                                              "Вы уверены, что хотите закрыть окно?",
                                              QMessageBox::Yes | QMessageBox::No);
        if (question == QMessageBox::Yes) {
            accept();
        }
    } else {
        accept();
    }
}
