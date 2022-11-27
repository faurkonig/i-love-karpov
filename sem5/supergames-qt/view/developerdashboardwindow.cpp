#include "developerdashboardwindow.h"
#include "ui_developerdashboardwindow.h"
#include "gamedialog.h"
#include <QDateTime>

DeveloperDashboardWindow::DeveloperDashboardWindow(SgDeveloper dev, QSqlDatabase *newDb, QWidget *parent) :
    QMainWindow(parent),
    DatabaseContainer(this, newDb),
    ui(new Ui::DeveloperDashboardWindow),
    devId(dev.id)
{
    ui->setupUi(this);

    updateStats();
    updateGameList();
}

DeveloperDashboardWindow::~DeveloperDashboardWindow()
{
    delete ui;
}

void DeveloperDashboardWindow::updateStats()
{
    bool ok;
    // Высчитываем время
    auto currentTime = QDateTime::currentDateTimeUtc();
    currentTime.setTime(QTime::fromMSecsSinceStartOfDay(0));
    auto dayAgo = currentTime.toSecsSinceEpoch();
    auto weekAgo = currentTime.addDays(-7).toSecsSinceEpoch();

    // Общий запрос для получения статистики по покупкам
    auto collectionQuery = QString("SELECT count(ce.id) "
                                   "FROM internal.collection_elements ce "
                                   "WHERE ce.\"date\" >= to_timestamp(%2) "
                                   "AND ce.game IN (SELECT id FROM public.games g "
                                   "WHERE g.developer = %1)").arg(devId);

    // Покупки за сегодня
    auto statQ = execQuery(collectionQuery.arg(dayAgo), ok);
    if (!ok) return;
    statQ.first();
    ui->sellsTodayLabel->setText(QString("Покупок за сегодня: <u>%1</u>")
                                 .arg(statQ.value(0).toInt()));

    // Покупки за неделю
    statQ = execQuery(collectionQuery.arg(weekAgo), ok);
    if (!ok) return;
    statQ.first();
    ui->sellsWeekLabel->setText(QString("Покупок за последние 7 дней: <u>%1</u>")
                                .arg(statQ.value(0).toInt()));

    // Покупки за всё время
    statQ = execQuery(collectionQuery.arg(0), ok);
    if (!ok) return;
    statQ.first();
    ui->sellsAllLabel->setText(QString("Покупок за всё время: <u>%1</u>")
                               .arg(statQ.value(0).toInt()));

    // Общий запрос для получения статистика по отзывам
    auto reviewQuery = QString("SELECT count(r.id) "
                               "FROM public.reviews r "
                               "WHERE r.\"date\" >= to_timestamp(%2) "
                               "AND r.game IN (SELECT id FROM public.games g "
                               "WHERE g.developer = %1)").arg(devId);

    // Отзывы за сегодня
    statQ = execQuery(reviewQuery.arg(dayAgo), ok);
    if (!ok) return;
    statQ.first();
    ui->reviewsTodayLabel->setText(QString("Отзывов за сегодня: <u>%1</u>")
                                   .arg(statQ.value(0).toInt()));

    // Отзывы за неделю
    statQ = execQuery(reviewQuery.arg(weekAgo), ok);
    if (!ok) return;
    statQ.first();
    ui->reviewsWeekLabel->setText(QString("Отзывов за последние 7 дней: <u>%1</u>")
                                  .arg(statQ.value(0).toInt()));

    // Отзывы за всё время
    statQ = execQuery(reviewQuery.arg(0), ok);
    if (!ok) return;
    statQ.first();
    ui->reviewsAllLabel->setText(QString("Отзывов за всё время: <u>%1</u>")
                                 .arg(statQ.value(0).toInt()));
}

void DeveloperDashboardWindow::updateGameList()
{
    bool ok;
    auto gamesQ = execQuery(QString("SELECT id, \"name\", description FROM public.games g "
                                    "WHERE g.developer = %1").arg(devId), ok);
    if (!ok) return;

    // Заранее очищаем все виджеты с экрана
    for (auto gi : qAsConst(gameItems)) {
        ui->gamesVerticalLayout->removeWidget(gi);
        gi->deleteLater();
    }
    gameItems.clear();

    if (gamesQ.size() < 1) {
        // Если не нашли игр у разработчика, то просто показываем информацию об этом
        ui->noGamesLabel->show();
        return;
    }

    // Скрываем надпись о том, что нет игр
    ui->noGamesLabel->hide();

    DevGameItem *gi;
    // Добавляем все игры как виджеты в список
    while (gamesQ.next()) {
        gi = new DevGameItem(gamesQ.value(0).toInt(),
                             gamesQ.value(1).toString(), gamesQ.value(2).toString(),
                             ui->gamesBlock);
        connect(gi, &DevGameItem::onOpenGameButtonPressed, this, &DeveloperDashboardWindow::onOpenGame);
        ui->gamesVerticalLayout->insertWidget(0, gi);
        gameItems.append(gi);
    }
}


void DeveloperDashboardWindow::on_updateStatsButton_clicked()
{
    updateStats();
}

void DeveloperDashboardWindow::onOpenGame(int gameId)
{
    GameDialog(mainDatabase, gameId, 0, this).exec();
}
