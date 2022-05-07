#include "Graph/SEQ/Box.h"

Box::Box(QGraphicsScene* parent, QRect rect_t, QString name) :rect(rect_t), name(name)
{
    setPos(rect.x(), rect.y());
    this->rect.setX(0);
    this->rect.setY(0);
    this->rect = this->rect.marginsAdded(BOX_OFF);
}

QRectF Box::boundingRect() const
{
    return rect.normalized().marginsAdded(BOX_OFF).marginsAdded(SEQ_BOUND_OF);
}

void Box::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    //draw bounds
    painter->drawRect(rect);
    //go to top center of bounds
    painter->translate(rect.width() / 2, 0);
    //offset to draw centered
    QRectF descRect = QFontMetrics(QApplication::font()).boundingRect(name);
    painter->translate(-descRect.width() / 2, -descRect.height() / 2);
    descRect += {qreal(metric.averageCharWidth()), 0, qreal(metric.averageCharWidth()), 0};

    auto fill = greenG(descRect.height());
    painter->setBrush(fill);

    painter->drawRect(descRect);
    painter->drawText(0, 0, name);

    drawDebug(painter, this);
}
