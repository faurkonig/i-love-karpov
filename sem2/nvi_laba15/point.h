#ifndef POINT_H
#define POINT_H


class Point
{
public:
    Point();

    bool setX(int newX);
    bool setY(int newY);
    bool setXY(int newX, int newY);
    int getX();
    int getY();
    void getXY(int &newX, int &newY);

private:
    int x;
    int y;
};

#endif // POINT_H
