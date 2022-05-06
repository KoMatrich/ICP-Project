#include "Graph/SEQ/Arrow.h"

SEQArrow::SEQArrow(QGraphicsScene* parent, const QPoint& pos1, const QPoint& pos2, const RuleID& arr_type, const QString& method, const int error)
    :pos1(pos1), pos2(pos2), method(method), has_error(error)
{
    arrow_type = arr_type;
    setParent(parent);

    update();
}

QRectF SEQArrow::boundingRect() const
{
    return QRect(pos().toPoint(), end.toPoint()).normalized();
}

void SEQArrow::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPoint textWidth{ metric.size(Qt::TextLongestVariant, method).width() - OFFSET,0 };
    painter->drawLine(end.toPoint(), col_vec.toPoint());
    painter->drawText((pos1 - pos2) / 2 - textWidth, method);

    if (arrow_type == RuleID::R_AGG)
        painter->setBrush(Qt::black);
    else
        painter->setBrush(Qt::white);

    painter->drawPolygon(arrow_head);
}

void SEQArrow::update()
{
    updateArrow();
    updateArrowHead();
}

void SEQArrow::updateArrow()
{
    //set arrow position to center of object 2
    setPos(pos2);

    //get connecting vector
    QVector2D end = QVector2D(pos1 - pos2);

    //get colision vector without arrow head size
    col_vec = QVector2D(end.x() - ACTIVATION_W * sgn(end.x()), end.y());
}

void SEQArrow::updateArrowHead()
{
    double angle = std::atan2(-col_vec.y(), col_vec.x());

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

void SEQArrow::destroy()
{
    delete this;
}
