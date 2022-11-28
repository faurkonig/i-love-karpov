#ifndef DEVELOPERDASHBOARDWINDOW_H
#define DEVELOPERDASHBOARDWINDOW_H

#include "sgdeveloper.h"
#include "utils/databasecontainer.h"
#include "components/devgameitem.h"
#include <QMainWindow>

namespace Ui {
class DeveloperDashboardWindow;
}

class DeveloperDashboardWindow : public QMainWindow, private DatabaseContainer
{
    Q_OBJECT

public:
    /// Конструктор
    explicit DeveloperDashboardWindow(SgDeveloper dev, QSqlDatabase *newDb, QWidget *parent = nullptr);
    /// Деструктор
    ~DeveloperDashboardWindow();

private slots:
    /// Обработчик нажатия на кнопку обновления статистики
    void on_updateStatsButton_clicked();

    /// Обработчик нажатия на кнопку добавления игры
    void on_addGameButton_clicked();

    /// Обработчик кнопки "Об игре" в списке игр
    void onOpenGame(int gameId);

    /// Обработчик кнопки "Редактировать" в списке игр
    void onEditGame(int gameId);

private:
    Ui::DeveloperDashboardWindow *ui;

    int devId;

    QList<DevGameItem *> gameItems;

    /// Метод для обновления статистики разработчика
    void updateStats();

    /// Метод для обновления списка игр, выпущенных разработчиком
    void updateGameList();
};

#endif // DEVELOPERDASHBOARDWINDOW_H
