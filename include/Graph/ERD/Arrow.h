#pragma once

#include "qobject.h"

class Arrow : public QGraphicsObject
{
    Q_OBJECT
public:
    Arrow(QGraphicsScene* parent, QGraphicsObject* o1, QGraphicsObject* o2);

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

private:
    const int OFFSET = 15;
    const QPointF POFFSET{ OFFSET,OFFSET };
    QPoint p1, s1, p2, s2;
};
