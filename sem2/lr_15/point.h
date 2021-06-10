#ifndef POINT_H
#define POINT_H


class Point
{
public:
    Point();

    bool setX(int new_x);
    bool setY(int new_y);
    bool setXY(int new_x, int new_y);
    int getX();
    int getY();
    void getXY(int &new_x, int &new_y);

private:
    int x;
    int y;
};

#endif // POINT_H
