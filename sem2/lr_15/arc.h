#ifndef ARC_H
#define ARC_H

#include <primitive.h>
#include <elliptic.h>


class Arc : public Primitive, public Elliptic
{
public:
    Arc();

    bool setStartAngle(int angle);
    bool setSpanAngle(int angle);
    int getStartAngle();
    int getSpanAngle();

    void draw(QImage * im) override;

private:
    int startAngle;
    int spanAngle;
};

#endif // ARC_H
