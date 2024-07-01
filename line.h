#ifndef LINE_H
#define LINE_H

#include "shape.h"
#include <QLineF>

class Line : public Shape {
public:
    Line(QPointF startPoint_, QPointF endPoint_, const QPen& pen_, const QBrush& brush_) : Shape(startPoint_, endPoint_, pen_, brush_){}

    ~Line() {}
protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override{
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setPen(pen);
        painter->setBrush(brush);

        painter->drawLine(startPoint, endPoint);
    }

    QRectF boundingRect() const override{
        return QRectF(startPoint, endPoint).normalized();
    }
};

#endif // LINE_H
