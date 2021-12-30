#ifndef MINVALUEARRAY_H
#define MINVALUEARRAY_H

#include <QVector>

class MinValueArray
{
public:
    MinValueArray();

    void addValue(int value);
    void clearAll();
    int minValue();
    int length();

private:
    QVector<int> values;
};

#endif // MINVALUEARRAY_H
