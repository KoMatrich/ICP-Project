#include "Graph/ERD/Arrow.h"

Arrow::Arrow(QGraphicsScene* parent, WItem* o1, WItem* o2)
    :o1(o1), o2(o1)
{
    //auto remove when one object is destroyed
    connect(o1, SIGNAL(destroyed()), this, SLOT(destroy()));
    connect(o1, SIGNAL(itemMoved()), this, SLOT(update()));

    connect(o2, SIGNAL(destroyed()), this, SLOT(destroy()));
    connect(o2, SIGNAL(itemMoved()), this, SLOT(update()));

    setParent(parent);

    update();
}

QRectF Arrow::boundingRect() const
{
    return QRect(-dist, dist);
}

void Arrow::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QVector<QLine> lines = { { -dist,dist} };
    painter->drawLines(lines);

    painter->drawRect(boundingRect());
}

void Arrow::update()
{
    auto p1 = o1->pos().toPoint();
    auto p2 = o2->pos().toPoint();

    s1 = o1->boundingRect().center().toPoint();
    s2 = o2->boundingRect().center().toPoint();

    dist = (p1 + s1) - (p2 + s2);

    this->setPos(p1);
}

void Arrow::destroy()
{
    delete this;
}
