#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include "databasecontainer.h"
#include <QDialog>

namespace Ui {
class GameDialog;
}

class GameDialog : public QDialog, private DatabaseContainer
{
    Q_OBJECT

public:
    /// Конструктор
    explicit GameDialog(QSqlDatabase *newDb, int id, int currentUser, QWidget *parent = nullptr);
    /// Деструктор
    ~GameDialog();

private slots:
    /// Обработчик нажатия на кнопку покупки/добавления игры
    void on_buyButton_clicked();

private:
    Ui::GameDialog *ui;

    /// ID игры, которую надо показать
    int gameId;
    /// ID пользователя,
    int userId;

    /// Цена игры (нужен для обновления на кнопке)
    int gamePrice;

    /// Флаг, отвечающий за присутствие игры в коллекции
    bool inCollection;
    /// Флаг, отвечающий за присутствие игры в корзине
    bool inCart;

    /// SQL запрос для получения игры и всей информации о ней для отображения
    QString gameQuerySql {
        "SELECT"
        " g.\"name\", g.description, g.price, g.\"date\","
        " d.\"name\" AS dev_name, d.description AS dev_description, d.email AS dev_email, d.\"date\" AS dev_date,"
        " (SELECT count(r.id) AS review_count"
        " FROM public.reviews r"
        " WHERE r.game = g.id),"
        " (SELECT count (ce.id) AS collection_count"
        " FROM internal.collection_elements ce"
        " WHERE ce.game = g.id) "
        "FROM public.games g "
        "JOIN (SELECT"
        "  id, \"name\", description, email, \"date\""
        " FROM public.developers) AS d"
        " ON d.id = g.developer "
        "WHERE g.id = %1"
    };
    /// SQL запрос для получения всех отзывов на игру
    QString gameReviewsQuerySql {
        "SELECT (SELECT u.\"name\" FROM public.users u"
        " WHERE u.id = r.\"user\"),"
        " r.rating, r.\"content\", r.\"date\" "
        "FROM public.reviews r "
        "WHERE r.game = %1 "
        "ORDER BY r.\"date\" DESC"
    };

    /// Метод для обновления данных внутри диалога
    void updateData();

    /// Метод для обновления кнопки покупки.
    /// Вызывается чаще, чем [updateData], так как данные даже без запроса надо
    /// обновлять по нажатию на кнопку.
    /// Поэтому метод и вынесен отдельно
    void updateBuyButton();
};

#endif // GAMEDIALOG_H
