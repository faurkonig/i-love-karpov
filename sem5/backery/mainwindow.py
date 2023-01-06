import sys

import PySide6.QtCore
from PySide6.QtWidgets import QApplication, QMainWindow, QTableWidget, QTableWidgetItem
from PySide6.QtCore import Slot

# pyside6-uic form.ui -o ui_form.py
from ui_form import Ui_MainWindow

from helpers.global_timer import GlobalTimer
import models.order


class MainWindow(QMainWindow):
    orders: list[models.order.Order] = []
    orderTimeout = models.order.randomOrderTimeout()

    def __init__(self, parent=None):
        super().__init__(parent)

        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        # Глобальный счётчик времени
        self._globalTimer = GlobalTimer(self)
        self._globalTimer.onTick.connect(self.timerTick)

    @Slot()
    def timerTick(self):
        formatedTime = self._globalTimer.currentTime.toString('hh:mm:ss')
        print('Timer ticking:', formatedTime)
        self.ui.timeLabel.setText(formatedTime)

        self.processOrders()

    def processOrders(self):
        self.orderTimeout -= 1
        if self.orderTimeout <= 0:
            # Если пришло время создавать новый заказ
            self.orderTimeout = models.order.randomOrderTimeout()
            newOrder = models.order.createNew()
            newOrder.orderTime = self._globalTimer.currentTime
            self.orders.append(newOrder)
            self.ui.ordersTable.setRowCount(len(self.orders))
            self._updateOrder(len(self.orders) - 1)

        # Обновляем текст с временем до следующего заказа
        self.ui.orderTimeLabel.setText(
            'Секунд до следующего заказа: <b>{}</b>'.format(self.orderTimeout))

    def _updateOrder(self, index: int):
        self._initRow(self.ui.ordersTable, index)
        order = self.orders[index]
        self.ui.ordersTable.item(index, 0).setText(order.id)
        self.ui.ordersTable.item(index, 1).setText(order.customerName)
        self.ui.ordersTable.item(index, 2).setText(str(order.orderSize))
        self.ui.ordersTable.item(index, 3).setText(str(order.readySize))
        if order.orderTime is not None:
            self.ui.ordersTable.item(index, 4).setText(
                order.orderTime.toString('hh:mm:ss')
            )
        if order.readyTime is not None:
            self.ui.ordersTable.item(index, 5).setText(
                order.readyTime.toString('hh:mm:ss')
            )
        statusItem = self.ui.ordersTable.item(index, 6)
        match order.status:
            case models.order.OrderState.created:
                statusItem.setText('Не готово')
            case models.order.OrderState.inProcess:
                statusItem.setText('Готовится')
            case models.order.OrderState.ready:
                statusItem.setText('Готово!')

    def _initRow(self, table: QTableWidget, y: int):
        for x in range(table.columnCount()):
            item = QTableWidgetItem()
            item.setFlags(item.flags() ^ (PySide6.QtCore.Qt.ItemFlag.ItemIsEditable |
                          PySide6.QtCore.Qt.ItemFlag.ItemIsSelectable |
                          PySide6.QtCore.Qt.ItemFlag.ItemIsUserCheckable))
            table.setItem(y, x, item)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    widget = MainWindow()
    widget.show()
    sys.exit(app.exec())
