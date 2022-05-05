#include "Graph/SEQ/Column.h"

Column::Column(QGraphicsScene* parent, QPoint pos, QString name, int cont_height)
    : name(name), cont_height(cont_height)
{
    setPos(pos);
    size = metric.size(Qt::TextLongestVariant, name);

    fill = redG(size.height());
}

QRectF Column::boundingRect() const
{
    return QRectF(QPoint{ 0,0 }, QPoint{ size.width(),size.height() + cont_height });
}

void Column::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPen thick_pen = QPen();
    thick_pen.setWidth(3);
    QPen selected_pen = QPen();
    selected_pen.setWidth(5);
    selected_pen.setColor(Qt::red);

    //draw background frame
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(fill);
    if (is_thick) {
        painter->setPen(thick_pen);
    }
    painter->drawRoundedRect(boundingRect(), RADIUS, RADIUS);
}

void Column::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
}
