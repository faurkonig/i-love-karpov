class BackeryStorage:
    collected = 0
    maximumStorage = 50
    totalCollected = 0

    def addBun(self, count=1):
        self.totalCollected += count
        # Ограничиваем переполнение
        newValue = min(self.collected + count, self.maximumStorage)
        delta = newValue - self.collected
        self.collected = newValue
        return delta

    def takeBun(self, count=1):
        self.collected -= count
        if self.collected < 0:
            self.collected = 0

    def hasSpace(self):
        return self.collected < self.maximumStorage

