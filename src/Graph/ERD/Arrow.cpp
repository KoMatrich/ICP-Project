#include "Graph/ERD/Arrow.h"

Arrow::Arrow(QGraphicsScene* parent, QGraphicsObject* o1, QGraphicsObject* o2)
{
    setParent(parent);

    p1 = o1->pos().toPoint();
    s1 = o1->boundingRect().center().toPoint();
    p2 = o2->pos().toPoint();
    s1 = o2->boundingRect().center().toPoint();
}

QRectF Arrow::boundingRect() const
{
    return QRectF(p1 - POFFSET, p2 + POFFSET);
}

void Arrow::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QVector<QLine> lines = { QLine(p1 + s1, p2 + s2) };
    painter->drawLines(lines);
}
