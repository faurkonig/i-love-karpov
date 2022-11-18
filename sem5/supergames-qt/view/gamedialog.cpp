#include "gamedialog.h"
#include "utils/commonpatterns.h"
#include "ui_gamedialog.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDateTime>

GameDialog::GameDialog(QSqlDatabase *newDb, int id, int currentUser, QWidget *parent) :
    QDialog(parent),
    DatabaseContainer(parent, newDb),
    ui(new Ui::GameDialog),
    gameId(id),
    userId(currentUser)
{
    ui->setupUi(this);

    updateData();

    // Блокируем кнопку покупки для гостевого профиля
    if (currentUser == 0) {
        ui->buyButton->setEnabled(false);
    }
}

GameDialog::~GameDialog()
{
    delete ui;
}

void GameDialog::updateData()
{
    bool ok;
    auto gameQ = execQuery(gameQuerySql.arg(gameId), ok);
    if (!ok) return;

    gameQ.first();

    // Собираем данные основного запроса
    auto gameName = gameQ.value(0).toString();
    auto gameDescription = gameQ.value(1).toString();
    gamePrice = gameQ.value(2).toDouble();
    auto gameDate = gameQ.value(3).toDateTime();
    auto devName = gameQ.value(4).toString();
    auto devDescription = gameQ.value(5).toString();
    auto devEmail = gameQ.value(6).toString();
    auto devDate = gameQ.value(7).toDateTime();
    auto reviewCount = gameQ.value(8).toInt();
    auto collectionCount = gameQ.value(9).toInt();

    // Выводим данные основного запроса
    ui->gameTitle->setText(gameName);
    ui->gameDescription->setText(gameDescription);
    ui->gameDeveloper->setText(QString("Разработана <u>%1</u>")
                               .arg(devName));
    ui->gameDate->setText(QString("Добавлена <u>%1</u>")
                          .arg(gameDate.toLocalTime().toString(*CommonPatterns::dateTimeFormat)));
    ui->gameCollectionCount->setText(QString("В коллекции у <u>%1</u> пользовател%2")
                                     .arg(collectionCount)
                                     .arg(collectionCount % 10 == 1 ? "я" : "ей"));
    ui->developerDescription->setText(devDescription);
    ui->developerDate->setText(QString("Зарегистрирован <u>%1</u>")
                               .arg(devDate.toLocalTime().toString(*CommonPatterns::dateTimeFormat)));
    ui->developerEmail->setText(devEmail);

    inCollection = inCart = false;
    // Проверяем, есть ли эта игра уже у нас в коллекции
    auto collectionQ = execQuery(QString("SELECT id FROM internal.collection_elements "
                                         "WHERE game = %1 AND \"user\" = %2")
                                 .arg(gameId).arg(userId), ok);
    if (!ok) return;

    if (collectionQ.size() > 0) {
        // Если есть в коллекции
        inCollection = true;
    } else {
        // Если нет в коллекции, то проверяем в корзине
        auto cartQ = execQuery(QString("SELECT id FROM internal.cart_elements "
                                       "WHERE game = %1 AND \"user\" = %2")
                               .arg(gameId).arg(userId), ok);
        if (!ok) return;

        if (cartQ.size() > 0) {
            // Если есть в корзине
            inCart = true;
        }
    }

    updateBuyButton();

    if (reviewCount > 0) {
        // Если уже известно, что на игру есть отзывы, то грузим их
        ui->noReviewLabel->hide();

        auto reviewsQ = execQuery(gameReviewsQuerySql.arg(gameId), ok);
        if (!ok) return;

        while (reviewsQ.next()) {
            auto userName = reviewsQ.value(0).toString();
            auto reviewRating = reviewsQ.value(1).toInt();
            auto reviewContent = reviewsQ.value(2).toString();
            auto reviewDate = reviewsQ.value(3).toDateTime();

            // Каждый виджет с отзывом - это просто QLabel с заданным текстом
            auto reviewText = QString("<b style=\"font-size: 15px\">%1</b> оставил отзыв %2:<br>"
                                      "<b style=\"font-size: 11px\">Оценка %3 из 10</b><br>%4")
                    .arg(userName, reviewDate.toString(*CommonPatterns::dateTimeFormat))
                    .arg(reviewRating).arg(reviewContent);

            auto label = new QLabel(reviewText, ui->reviewGroupBox);
            label->setWordWrap(true);
            ui->reviewsVerticalLayout->addWidget(label);
        }
    }
}

void GameDialog::on_buyButton_clicked()
{
    bool ok;
    if (gamePrice > 0) {
        // Если игра не бесплатная
        if (inCart) {
            // Удаляем игру из корзины
            execQuery(QString("DELETE FROM internal.cart_elements "
                              "WHERE game = %1 AND \"user\" = %2")
                      .arg(gameId).arg(userId), ok);
            if (!ok) return;

            inCart = false;
        } else {
            // Добавляем игру в корзину
            execQuery(QString("INSERT INTO internal.cart_elements (game, \"user\") "
                              "VALUES (%1, %2)")
                      .arg(gameId).arg(userId), ok);
            if (!ok) return;

            inCart = true;
        }
    } else {
        // Если игра бесплатная
        // Добавляем игру в коллекцию
        execQuery(QString("INSERT INTO internal.collection_elements (game, \"user\") "
                          "VALUES (%1, %2)")
                  .arg(gameId).arg(userId), ok);
        if (!ok) return;

        inCollection = true;
    }

    updateBuyButton();
}

void GameDialog::updateBuyButton()
{
    if (inCollection) {
        // Если игра уже в коллекции, то просто отображаем это и скрываем кнопку
        ui->buyButton->setEnabled(false);
        ui->buyButton->setText("Уже в коллекции");
        return;
    }

    if (gamePrice > 0) {
        // Если игра не бесплатная
        if (inCart) {
            ui->buyButton->setText("Убрать из корзины");
        } else {
            ui->buyButton->setText(QString("Добавить в корзину (%1 руб.)")
                                   .arg(gamePrice));
        }
    } else {
        // Если игра бесплатная
        ui->buyButton->setText("Добавить в коллекцию (Бесплатно)");
    }
}

