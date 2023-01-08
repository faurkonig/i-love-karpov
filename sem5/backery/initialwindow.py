from typing import Optional

import PySide6.QtCore
from PySide6.QtWidgets import QDialog
from PySide6.QtCore import Slot

# pyside6-uic welcome.ui -o ui_welcome.py
from ui_welcome import Ui_Dialog

import constants


class InitialConfig:
    backers = constants.initialBackers
    couriers = constants.initialCouriers
    storage = constants.initialStorage

    maxMoney = constants.initialMoney

    isAccepted = False

    def calculateMoney(self):
        backersCost = (self.backers - constants.initialBackers) * \
            constants.backersCost
        courierCost = (self.couriers - constants.initialCouriers) * \
            constants.courierCost
        storageCost = (self.storage - constants.initialStorage) * \
            constants.storageCost
        return self.maxMoney - (backersCost + courierCost + storageCost)


class InitialWindow(QDialog):
    def __init__(self, config: InitialConfig, parent=None, f=PySide6.QtCore.Qt.WindowType.Dialog) -> None:
        super().__init__(parent, f)

        self.ui = Ui_Dialog()
        self.ui.setupUi(self)

        self.ui.acceptButton.clicked.connect(self._onDialogAccepted)
        self.ui.backersSpinBox.valueChanged.connect(self._onBackerCountChanged)
        self.ui.couriersSpinBox.valueChanged.connect(
            self._onCourierCountChanged)
        self.ui.storageSpinBox.valueChanged.connect(
            self._onStorageCountChanged)

        self.config = config
        self._updateInitialContent()
        self._updateMutableContent()

    @Slot()
    def _onBackerCountChanged(self, value: int):
        print('Backers changed to', value)
        self.config.backers = value
        self._updateMutableContent()

    @Slot()
    def _onCourierCountChanged(self, value: int):
        print('Courier changed to', value)
        self.config.couriers = value
        self._updateMutableContent()

    @Slot()
    def _onStorageCountChanged(self, value: int):
        print('Storage changed to', value)
        self.config.storage = value
        self._updateMutableContent()

    @Slot()
    def _onDialogAccepted(self):
        if self.config.calculateMoney() >= 0:
            self.config.isAccepted = True
            self.close()

    def _updateInitialContent(self):
        self.ui.backersSpinBox.setMinimum(self.config.backers)
        self.ui.backersSpinBox.setMaximum(constants.maxInitialBackers)
        self.ui.backersLabel.setText(
            'Нанять пекаря <i>({} $)</i>'.format(constants.backersCost))

        self.ui.couriersSpinBox.setMinimum(self.config.couriers)
        self.ui.couriersSpinBox.setMaximum(constants.maxInitialCouriers)
        self.ui.couriersLabel.setText(
            'Нанять курьера <i>({} $)</i>'.format(constants.courierCost))

        self.ui.storageSpinBox.setMinimum(self.config.storage)
        self.ui.storageSpinBox.setMaximum(constants.maxInitialStorage)
        self.ui.storageLabel.setText(
            'Увеличить склад <i>({} $)</i>'.format(constants.storageCost))

    def _updateMutableContent(self):
        money = self.config.calculateMoney()
        if money >= 0:
            moneyText = 'Доступно денег: <b>{}</b> из <b>{}</b> $'
            self.ui.acceptButton.setEnabled(True)
        else:
            moneyText = 'Доступно денег: <b style="color: red">{}</b> из <b>{}</b> $'
            self.ui.acceptButton.setEnabled(False)
        self.ui.moneyLabel.setText(
            moneyText.format(money, self.config.maxMoney))
