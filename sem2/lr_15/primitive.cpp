#include "primitive.h"

#include "pixel.h"
#include "line.h"
#include "rectangle.h"
#include "ellipse.h"
#include "arc.h"
#include "curve.h"
#include "polygon.h"

Primitive::Primitive()
{
    // Конструктор
}

//void Primitive::draw(QImage * im)
//{
//    switch (type) {
//    case PrimitiveTypes::PixelT: {
//        Pixel * obj = static_cast<Pixel *>(this);
//        obj->draw(im);
//        break;
//    }
//    case PrimitiveTypes::LineT: {
//        Line * obj = static_cast<Line *>(this);
//        obj->draw(im);
//        break;
//    }
//    case PrimitiveTypes::RectangleT: {
//        Rectangle * obj = static_cast<Rectangle *>(this);
//        obj->draw(im);
//        break;
//    }
//    case PrimitiveTypes::EllipseT: {
//        Ellipse * obj = static_cast<Ellipse *>(this);
//        obj->draw(im);
//        break;
//    }
//    case PrimitiveTypes::ArcT: {
//        Arc * obj = static_cast<Arc *>(this);
//        obj->draw(im);
//        break;
//    }
//    case PrimitiveTypes::CurveT: {
//        Curve * obj = static_cast<Curve *>(this);
//        obj->draw(im);
//        break;
//    }
//    case PrimitiveTypes::PolygonT: {
//        Polygon * obj = static_cast<Polygon *>(this);
//        obj->draw(im);
//        break;
//    }
//    }
//}
