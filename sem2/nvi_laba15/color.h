#ifndef COLOR_H
#define COLOR_H


class Color
{
public:
    Color();

    bool setRGBA(int r, int g, int b, int a = 255);
    int getRed();
    int getGreen();
    int getBlue();
    int getAlpha();
    void getRGBA(int &r, int &g, int &b, int &a);

private:
    int red;
    int green;
    int blue;
    int alpha;
};

#endif // COLOR_H
