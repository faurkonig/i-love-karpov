#include "reviewdialog.h"
#include "ui_reviewdialog.h"
#include "utils/commonpatterns.h"
#include <QDateTime>

ReviewDialog::ReviewDialog(int userId, int gameId, QSqlDatabase *newDb, QWidget *parent) :
    QDialog(parent),
    DatabaseContainer(this, newDb),
    ui(new Ui::ReviewDialog),
    userId(userId),
    gameId(gameId)
{
    ui->setupUi(this);

    updateContent();
}

ReviewDialog::~ReviewDialog()
{
    delete ui;
}


void ReviewDialog::updateContent()
{
    bool ok;
    auto gameQ = execQuery(QString("SELECT \"name\" FROM public.games "
                                   "WHERE id = %1").arg(gameId), ok);
    if (!ok || gameQ.size() < 1) {
        // Если вдруг такая игра вообще не найдена
        isExists = false;

        resetContent();
        return;
    }

    // Обновляем имя игры
    gameQ.first();
    ui->titleLabel->setText(QString("Отзыв на \"%1\"").arg(gameQ.value(0).toString()));

    auto reviewQ = execQuery(QString("SELECT id, rating, content, \"date\" FROM public.reviews "
                                     "WHERE game = %1 AND \"user\" = %2")
                             .arg(gameId).arg(userId), ok);
    if (!ok || reviewQ.size() < 1) {
        // Если отзыв от этого пользователя на эту игру не был найден
        isExists = false;

        // Сбрасываем данные
        resetContent();
        return;
    }

    // Показываем виджеты, которые могут быть скрыты
    ui->deleteButton->show();
    ui->dateLabel->show();

    // Иначе мы подразумеваем, что такой отзыв есть
    reviewQ.first();
    reviewId = reviewQ.value(0).toInt();
    // Обновляем содержимое виджетов
    ui->ratingSpinBox->setValue(reviewQ.value(1).toInt());
    ui->contentField->setText(reviewQ.value(2).toString());
    ui->dateLabel->setText(QString("Последнее обновление <u>%1</u>")
                           .arg(reviewQ.value(3).toDateTime().toLocalTime()
                                .toString(CommonPatterns::dateTimeFormat)));

    isExists = true;
}

void ReviewDialog::resetContent()
{
    ui->deleteButton->hide();
    ui->dateLabel->hide();
    ui->ratingSpinBox->setValue(1);
    ui->contentField->clear();
}


void ReviewDialog::on_deleteButton_clicked()
{
    bool ok;
    execQuery(QString("DELETE FROM public.reviews "
                      "WHERE id = %1").arg(reviewId), ok);

    // Если удаление прошло успешно, то обновляем страницу
    if (ok) updateContent();
}

void ReviewDialog::on_cancelButton_clicked()
{
    reject();
}


void ReviewDialog::on_saveButton_clicked()
{
    bool ok;
    auto rating = ui->ratingSpinBox->value();
    auto content = ui->contentField->toPlainText();

    if (isExists) {
        execQuery(QString("UPDATE public.reviews "
                          "SET rating = %1, content = '%2', \"date\" = now() "
                          "WHERE id = %3").arg(rating).arg(content).arg(reviewId), ok);
        if (ok) updateContent();
    } else {
        execQuery(QString("INSERT INTO public.reviews (game, \"user\", rating, content) "
                          "VALUES (%1, %2, %3, '%4')").arg(gameId).arg(userId)
                  .arg(rating).arg(content), ok);
        if (ok) updateContent();
    }
}
