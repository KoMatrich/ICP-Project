#pragma once

#include "qobject.h"
#include "Graph/ERD/Item.h"

class Arrow : public QGraphicsObject
{
    Q_OBJECT
public:
    Arrow(QGraphicsScene* parent, WItem* o1, WItem* o2);

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

private:
    const int OFFSET = 15;
    const QPointF POFFSET{ OFFSET,OFFSET };
    QPoint dist, s1, s2;
    WItem* o1;
    WItem* o2;

private slots:
    void destroy();
    void update();
};
