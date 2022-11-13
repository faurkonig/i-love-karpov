#ifndef FRIENDITEM_H
#define FRIENDITEM_H

#include <QWidget>

namespace Ui {
class FriendItem;
}

class FriendItem : public QWidget
{
    Q_OBJECT

public:
    explicit FriendItem(int friendship, QString userName, QWidget *parent = nullptr);
    ~FriendItem();

signals:
    void onDeleteButtonPressed(int friendshipId);

private slots:
    void on_deleteButton_clicked();

private:
    Ui::FriendItem *ui;

    int friendshipId;
};

#endif // FRIENDITEM_H
