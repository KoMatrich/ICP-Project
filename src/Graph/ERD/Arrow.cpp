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
   return QRect(this->pos().toPoint(), this->end.toPoint()).normalized(); //.marginsAdded(MARGIN)
}

void Arrow::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
#ifdef DEBUG_DRAW
    painter->drawEllipse({ 0,0 }, 10, 10);

    painter->drawEllipse(end.toPoint(), 10, 10);
    painter->drawEllipse(col_vec.toPointF(), 10, 10);
#endif // DEBUG_DRAW

    //painter->drawLine(centerLine);
}

void Arrow::update()
{
    //get pos of center of objects
    QPoint p1 = o1->pos().toPoint() + o1->boundingRect().center().toPoint();
    QPoint p2 = o2->pos().toPoint() + o2->boundingRect().center().toPoint();

    //set arrow position to center of object 1
    this->setPos(p2);

    //get connecting vector
    this->end = QVector2D(p1 - p2);

    QPoint col;
    auto w2 = o2->boundingRect().width() / 2 + 2;
    auto h2 = o2->boundingRect().height() / 2 + 2;

    QVector2D col_vec_w = this->end * w2 / std::abs(this->end.x());
    QVector2D col_vec_h = this->end * h2 / std::abs(this->end.y());

    if (std::abs(col_vec_w.y()) > h2)
        col_vec = col_vec_h;
    else
        col_vec = col_vec_w;
}

void Arrow::destroy()
{
    delete this;
}
