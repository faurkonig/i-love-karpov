import random
import string

from PySide6.QtCore import QTime
from enum import Enum

from helpers.names import getRandomName


class Order:
    orderTime: QTime | None = None
    readyTime: QTime | None = None

    def __init__(self, id: str, customerName: str, size: int):
        self.id = id
        self.customerName = customerName
        self.orderSize = size
        self.readySize = 0
        self.status = OrderState.created


class OrderState(Enum):
    created = 0
    inProcess = 1
    ready = 2


def createNew():
    orderId = random.choice(string.ascii_letters).capitalize(
    ) + '-' + str(random.randint(1000, 9999))
    return Order(orderId,
                 getRandomName(),
                 random.randint(1, 5))


def randomOrderTimeout():
    return random.randint(10, 20)
