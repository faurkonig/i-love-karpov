#ifndef VALUESARRAY_H
#define VALUESARRAY_H

#include <QVector>

class ValuesArray
{
public:
    ValuesArray();

    void addValue(int value);
    void clearAll();
    int negativeSum();
    int length();

private:
    QVector<int> values;
};

#endif // VALUESARRAY_H
