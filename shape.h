#ifndef SHAPE_H
#define SHAPE_H

#include <QGraphicsItem>
#include <QPropertyAnimation>
#include <QPainter>
#include <QObject>
#include <QPen>
#include <QBrush>

enum class ShapeType{
    None,
    Line,
    CurvedLine,
    Rectangle,
    Circle
};

class Shape : public QGraphicsItem{
public:
    Shape(QPointF startPoint_, QPointF endPoint_, const QPen& pen_, const QBrush& brush_) : QGraphicsItem(){
        startPoint = startPoint_;
        endPoint = endPoint_;
        pen = pen_;
        brush = brush_;
    }

    virtual ~Shape() {}

    virtual QRectF boundingRect() const override = 0;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override = 0;

    QPointF getStartPoint() const{
        return startPoint;
    }

    QPointF getEndPoint() const{
        return endPoint;
    }

    QColor getPenColor() const{
        return pen.color();
    }

    QColor getBrushColor() const{
        return brush.color();
    }

    void setStartPoint(const QPointF& point){
        startPoint = point;
        update();
    }

    void setEndPoint(const QPointF& point){
        endPoint = point;
        update();
    }

    void setPenColor(QColor color_){
        pen.setColor(color_);
        update();
    }

    void setBrushColor(QColor color_){
        brush.setColor(color_);
        update();
    }


protected:
    QPen pen;
    QBrush brush;

    QPointF startPoint;
    QPointF endPoint;
};

#endif // SHAPE_H
