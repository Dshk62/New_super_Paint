#ifndef CIRCLE_H
#define CIRCLE_H

#include "shape.h"

class Circle : public Shape {
public:
    Circle(QPointF startPoint_, QPointF endPoint_, const QPen& pen_, const QBrush& brush_) : Shape(startPoint_, endPoint_, pen_, brush_) {}

    ~Circle() {}

protected:
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr) override {
        Q_UNUSED(option);
        Q_UNUSED(widget);

        painter->setPen(pen);
        painter->setBrush(brush);

        qreal width = qAbs(endPoint.x() - startPoint.x());
        qreal height = qAbs(endPoint.y() - startPoint.y());

        QPointF topLeft(qMin(startPoint.x(), endPoint.x()), qMin(startPoint.y(), endPoint.y()));

        painter->drawEllipse(QRectF(topLeft, QSizeF(width, height)));
    }

    QRectF boundingRect() const override {
        qreal width = qAbs(endPoint.x() - startPoint.x());
        qreal height = qAbs(endPoint.y() - startPoint.y());

        QPointF topLeft(qMin(startPoint.x(), endPoint.x()), qMin(startPoint.y(), endPoint.y()));

        return QRectF(topLeft, QSizeF(width, height)).normalized();
    }
};

#endif // CIRCLE_H
