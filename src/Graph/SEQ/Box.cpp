#include "Graph/SEQ/Box.h"

Box::Box(QGraphicsScene* parent, QRect rect_t, QString name) :rect(rect_t), name(name)
{
    setPos(rect_t.x(), rect_t.y());
    rect.setX(0);
    rect.setY(0);
    rect = rect.marginsAdded(BOX_OFF);
}

QRectF Box::boundingRect() const
{
    return rect.normalized().marginsAdded(BOX_OFF).marginsAdded(SEQ_BOUND_OF);
}

void Box::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->drawRect(rect);

    QPoint textOffset{ 0, -30 };
    painter->drawText(textOffset, name);

    drawDebug(painter, this);
}
