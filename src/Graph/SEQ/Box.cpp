#include "Graph/SEQ/Box.h"

Box::Box(QGraphicsScene* parent, QRect rect) :rect(rect) {}

QRectF Box::boundingRect() const
{
    return rect.normalized().marginsAdded(BOUND_OF);
}

void Box::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawRect(rect);
}
