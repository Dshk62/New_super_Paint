#ifndef CURVEDLINE_H
#define CURVEDLINE_H

#include "shape.h"
#include <QPainterPath>
#include <random>

class CurvedLine : public Shape {
public:
    CurvedLine(QPointF startPoint_, QPointF endPoint_, const QPen& pen_, const QBrush& brush_) : Shape(startPoint_, endPoint_, pen_, brush_) {
        generateControlPoints();
    }

    ~CurvedLine() {}

    void setControlPoints(const QPointF& controlPoint1_, const QPointF& controlPoint2_) {
        controlPoint1 = controlPoint1_;
        controlPoint2 = controlPoint2_;
        update();
    }


protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setPen(pen);
        painter->setBrush(brush);

        QPainterPath path;
        path.moveTo(startPoint);
        path.cubicTo(controlPoint1, controlPoint2, endPoint);

        painter->drawPath(path);
    }

    QRectF boundingRect() const override {
        return QRectF(startPoint, endPoint).normalized();
    }

    void generateControlPoints() {
        controlPoint1 = QPointF(rand()% 801, rand()% 601);

        controlPoint2 = QPointF(rand()% 801, rand()% 601);
    }

private:
    QPointF controlPoint1;
    QPointF controlPoint2;
};

#endif // CURVEDLINE_H
