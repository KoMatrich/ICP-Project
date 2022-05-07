#include "Graph/ERD/ERDArrow.h"

ERDArrow::ERDArrow(QGraphicsScene* parent, QGraphicsObject* o1, QGraphicsObject* o2, RuleID arr_type)
    :o1(o1), o2(o2)
{
    setFlag(QGraphicsItem::ItemStacksBehindParent);
    connect(o1, SIGNAL(destroyed()), this, SLOT(destroy()));
    connect(o1, SIGNAL(itemMoved()), this, SLOT(update()), Qt::QueuedConnection);

    connect(o2, SIGNAL(destroyed()), this, SLOT(destroy()));
    connect(o2, SIGNAL(itemMoved()), this, SLOT(update()), Qt::QueuedConnection);

    this->arrow_type = arr_type;
    setParent(parent);

    update();
}

QRectF ERDArrow::boundingRect() const
{
    auto posP = pos().toPoint();
    auto vecP = col_vec.toPoint();
    return QRect(posP, vecP).normalized().marginsAdded(BOUND_OF);
}

void ERDArrow::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // if not self referencing:
    if (o1 != o2) {
        painter->drawLine(end.toPoint(), col_vec.toPoint());
    }
    // otherwise, create an 'U' Shape
    else {
        int x_off = o1->boundingRect().width() / 2 + ARROW_COMAGG_SIZE * 2;
        int y_off = o1->boundingRect().height() / 2 + ARROW_COMAGG_SIZE * 3;
        painter->drawRect(end.toPoint().x(), end.toPoint().y(), x_off, y_off);
    }

    if (arrow_type == RuleID::R_AGG)
        painter->setBrush(Qt::black);
    else
        painter->setBrush(Qt::white);

    painter->drawPolygon(arrow_head);

    drawDebug(painter, this);
}

void ERDArrow::update()
{
    updateArrow();
    updateArrowHead();
}

void ERDArrow::updateArrow()
{
    //get pos of center of objects
    QPoint p1 = o1->pos().toPoint() + o1->boundingRect().center().toPoint();
    QPoint p2 = o2->pos().toPoint() + o2->boundingRect().center().toPoint();

    //set arrow position to center of object 2
    this->setPos(p2);

    //get connecting vector
    this->end = QVector2D(p1 - p2);

    //get mid size coords
    auto w2 = o2->boundingRect().width() / 2 + ARROW_OFFSET;
    auto h2 = o2->boundingRect().height() / 2 + ARROW_OFFSET;

    // get coordinates of collisions by scaling the vector to size of boundrect
    QVector2D col_vec_w = this->end * w2 / std::abs(this->end.x());
    QVector2D col_vec_h = this->end * h2 / std::abs(this->end.y());

    //choose the correct collision
    if (std::abs(col_vec_w.y()) > h2)
        col_vec = col_vec_h;
    else
        col_vec = col_vec_w;
}

void ERDArrow::updateArrowHead()
{
    double angle = std::atan2(-col_vec.y(), col_vec.x());

    if (o1 == o2) {
        angle = std::atan2(-1, 0);
        col_vec = end + QVector2D(0, o1->boundingRect().height() / 2 + ARROW_OFFSET);
    }

    switch (arrow_type) {
    case RuleID::R_ASS:
        arrow_head.clear();
        return;
    case RuleID::R_COM:
    case RuleID::R_AGG:
    {
        QPointF arrowP1 = col_vec.toPointF() + QPointF(sin(angle + M_PI / 3) * ARROW_COMAGG_SIZE,
            cos(angle + M_PI / 3) * ARROW_COMAGG_SIZE);

        QPointF arrowP3 = col_vec.toPointF() + QPointF(sin(angle + 2 * M_PI / 3) * ARROW_COMAGG_SIZE,
            cos(angle + 2 * M_PI / 3) * ARROW_COMAGG_SIZE);

        QPointF arrowP2 = col_vec.toPointF() + QPointF(sin(angle + M_PI_2) * 2 * ARROW_COMAGG_SIZE,
            cos(angle + M_PI_2) * 2 * ARROW_COMAGG_SIZE);

        arrow_head.clear();
        arrow_head << col_vec.toPointF() << arrowP1 << arrowP2 << arrowP3;
        break;
    }
    case RuleID::R_GEN:
    {
        QPointF arrowP1 = col_vec.toPointF() + QPointF(sin(angle + M_PI / 3) * ARROW_GEN_SIZE,
            cos(angle + M_PI / 3) * ARROW_GEN_SIZE);

        QPointF arrowP2 = col_vec.toPointF() + QPointF(sin(angle + 2 * M_PI / 3) * ARROW_GEN_SIZE,
            cos(angle + 2 * M_PI / 3) * ARROW_GEN_SIZE);

        arrow_head.clear();
        arrow_head << col_vec.toPointF() << arrowP1 << arrowP2;
        break;
    }
    default:
        break;
    }
}

void ERDArrow::destroy()
{
    delete this;
}
