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
    return QRect(0, -(ACTION_RH+ACTION_H)/2, col_vec.toPoint().x(), ACTION_RH).normalized().marginsAdded(BOUND_OF);
}

void SEQArrow::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPoint textWidth{ metric.size(Qt::TextLongestVariant, method).width() - OFFSET,0 };

    QPen pen = QPen();

    if (has_error) {
        pen.setColor(Qt::red);
    } else {
        pen.setColor(Qt::black);
    }

    if (arrow_type == RuleID::R_ARROW_ASYNC)
        pen.setStyle(Qt::DashLine);
    else
        pen.setStyle(Qt::SolidLine);

    QBrush brush = QBrush(Qt::white);

    painter->setPen(pen);
    painter->setBrush(brush);

    QPoint textOffset{ metric.width(method) / 2, metric.height() / 2 };

    // if not self referencing:
    if (pos1 != pos2) {
        painter->drawLine(end.toPoint(), col_vec.toPoint());
    }
    else {
        painter->drawLine(end.toPoint(), end.toPoint() + QPointF(SELF_ARROW_WIDTH, 0));
        painter->drawLine(end.toPoint() + QPointF(SELF_ARROW_WIDTH, 0), end.toPoint() + QPointF(SELF_ARROW_WIDTH, SELF_ARROW_HEIGHT));
        painter->drawLine(end.toPoint() + QPointF(SELF_ARROW_WIDTH, SELF_ARROW_HEIGHT), end.toPoint() + QPointF(0, SELF_ARROW_HEIGHT));
        arrow_head.translate(QPointF(0, SELF_ARROW_HEIGHT));
    }

    painter->drawPolygon(arrow_head);

    if (abs(pos1.x() - pos2.x())>0) {
        painter->translate((pos1 - pos2) / 2 - textOffset);
    } else {
        painter->translate(QPoint{ int(ARROW_GEN_SIZE),-metric.height()/2 });
    }
    QRect descRect{ 0,0,metric.width(method),metric.height() };
    painter->drawRect(descRect.marginsAdded(BOUND_OF));
    painter->drawText(QPoint{ 0,metric.height() }, method);

    drawDebug(painter, this);
}

void SEQArrow::update()
{
    updateArrow();
    updateArrowHead();
}

void SEQArrow::updateArrow()
{
    //set arrow position to center of object 
    setPos(pos2);

    //get connecting vector
    QVector2D end = QVector2D(pos1 - pos2);

    //get colision vector without arrow head size
    col_vec = QVector2D(end.x(), end.y());
}

void SEQArrow::updateArrowHead()
{
    double angle = std::atan2(-col_vec.y(), col_vec.x());

    QPointF arrowP1 = end.toPointF() + QPointF(sin(angle + M_PI / 3) * ARROW_SEQ_SIZE,
        cos(angle + M_PI / 3) * ARROW_SEQ_SIZE);

    QPointF arrowP2 = end.toPointF() + QPointF(sin(angle + 2 * M_PI / 3) * ARROW_SEQ_SIZE,
        cos(angle + 2 * M_PI / 3) * ARROW_SEQ_SIZE);

    arrow_head.clear();
    arrow_head << end.toPointF() << arrowP1 << arrowP2;
}

void SEQArrow::destroy()
{
    delete this;
}
