#include "Graph/SEQ/Box.h"

Box::Box(QGraphicsScene* parent, QRect rect_t) :rect(rect_t)
{
    setPos(rect_t.x(), rect_t.y());
    rect.setX(0);
    rect.setY(0);
    rect = rect.marginsAdded(BOX_OFF);
}

QRectF Box::boundingRect() const
{
    return rect.normalized().marginsAdded(BOX_OFF).marginsAdded(BOUND_OF);
}

void Box::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawRect(rect);

    drawDebug(painter, this);
}
