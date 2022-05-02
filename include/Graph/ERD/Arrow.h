#pragma once

#include "qobject.h"
#include "Graph/ERD/Item.h"
#include "qmargins.h"

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
    QMargins renderMargin{ OFFSET, OFFSET, OFFSET, OFFSET };
    const QPointF POFFSET{ OFFSET,OFFSET };

    //vector that points to first target
    QPoint dist, s1, s2;

    //pointers to targets
    WItem* o1;
    WItem* o2;

private slots:
    void destroy();
    void update();
};
