#include "Graph/ERD/Arrow.h"

Arrow::Arrow(QGraphicsScene* parent, WItem* o1, WItem* o2)
    :o1(o1), o2(o2)
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
    return QRect(dist1_vec.toPoint(), dist2_vec.toPoint()).normalized().marginsAdded(MARGIN);
}

void Arrow::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
#ifdef DEBUG_DRAW
    painter->drawEllipse({ 0,0 }, 10, 10);

    painter->drawEllipse(dist1_vec.toPoint(), 10, 10);
    painter->drawEllipse(dist2_vec.toPoint(), 10, 10);
#endif // DEBUG_DRAW

    painter->drawLine(centerLine);
}

inline void setVecHeading(QVector2D& vec, QVector2D heading)
{
    uint magSq = vec.length();

    auto target_angle = qTan(heading.y() / heading.x());

    vec.setX(magSq * qAcos(target_angle));
    vec.setY(magSq * qAsin(target_angle));
}

void Arrow::update()
{
    //get pos of center of objects
    auto p1 = o1->pos().toPoint() + o1->boundingRect().center().toPoint();
    auto p2 = o2->pos().toPoint() + o2->boundingRect().center().toPoint();
    //set arrow position to be between
    this->setPos((p1 + p2) / 2);
    //calculate distance / 2
    auto dif_p = (p1 - p2) / 2;
    //set distance vectors
    dist1_vec = QVector2D(dif_p);
    dist2_vec = QVector2D(-dif_p);
    //create test line
    centerLine = { dist1_vec.toPoint(), dist2_vec.toPoint() };
    //magic limit
    //update line
    centerLine = { dist1_vec.toPoint(), dist2_vec.toPoint() };
}

void Arrow::destroy()
{
    delete this;
}
