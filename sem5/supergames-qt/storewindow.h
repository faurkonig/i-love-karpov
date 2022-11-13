#ifndef STOREWINDOW_H
#define STOREWINDOW_H

#include "databasecontainer.h"
#include "sguser.h"
#include <QMainWindow>

namespace Ui {
class StoreWindow;
}

class StoreWindow : public QMainWindow, private DatabaseContainer
{
    Q_OBJECT

public:
    explicit StoreWindow(SgUser user, QSqlDatabase *newDb, QWidget *parent = nullptr);
    ~StoreWindow();

private slots:
    void on_updateButton_clicked();

private:
    Ui::StoreWindow *ui;

    SgUser user;

    QList<QWidget *> gameItems;

    const QString gamesQuerySql{
        "SELECT"
        " g.id,"
        " g.\"name\","
        " g.description,"
        " g.price,"
        " ("
        " SELECT"
        "  d.name AS developer"
        " FROM"
        "  public.developers d"
        " WHERE"
        "  g.developer = d.id"
        " )"
        "FROM"
        " public.games g;"
    };

    void updateGames();

    void openGame(int gameId);
};

#endif // STOREWINDOW_H