#ifndef MAXVALUEARRAY_H
#define MAXVALUEARRAY_H

#include <QVector>

class MaxValueArray
{
public:
    MaxValueArray();

    void addValue(int value);
    void clearAll();
    int maxValue();
    int length();

private:
    QVector<int> values;
};

#endif // MAXVALUEARRAY_H
