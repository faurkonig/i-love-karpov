#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include "databasecontainer.h"
#include "sguser.h"
#include <QDialog>

namespace Ui {
class GameDialog;
}

class GameDialog : public QDialog, private DatabaseContainer
{
    Q_OBJECT

public:
    explicit GameDialog(QSqlDatabase *newDb, int id, SgUser user, QWidget *parent = nullptr);
    ~GameDialog();

private:
    Ui::GameDialog *ui;

    int gameId;
    SgUser user;

    QString gameQuerySql {
        "SELECT"
        " g.\"name\","
        " g.description,"
        " g.price,"
        " g.\"date\","
        " d.\"name\" AS dev_name,"
        " d.description AS dev_description,"
        " d.email AS dev_email,"
        " d.\"date\" AS dev_date,"
        " (SELECT"
        "  count(r.id) AS review_count"
        " FROM"
        "  public.reviews r"
        " WHERE"
        "  r.game = g.id),"
        " (SELECT"
        "  count (ce.id) AS collection_count"
        " FROM"
        "  internal.collection_elements ce"
        " WHERE"
        "  ce.game = g.id) "
        "FROM"
        " public.games g "
        "JOIN ("
        " SELECT"
        "  id,"
        "  \"name\","
        "  description,"
        "  email,"
        "  \"date\""
        " FROM"
        "  public.developers) AS d"
        " ON"
        " d.id = g.developer "
        "WHERE"
        " g.id = %1"
    };

    void updateData();
};

#endif // GAMEDIALOG_H
