#include "Graph/SEQ/Column.h"

Column::Column(QGraphicsScene* parent, SEQMember& mem, const int& header_height, const int& height)
    : cont_height(height), name(mem.getName())
{
    size = metric.size(Qt::TextLongestVariant, name);

    rsize = QSize{ size.width(), header_height } + SOFFSET;

    fill = greenG(size.height());

    activations = mem.getActivations();
}

QRectF Column::boundingRect() const
{
    return QRectF(0, 0, rsize.width(), rsize.height() + cont_height);
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
    //align to center
    painter->translate(-rsize.width() / 2, 0);
    //draw rectangle in center
    painter->drawRoundedRect(QRect(0, 0, rsize.width(), rsize.height()), RADIUS, RADIUS);
    //draw text in center of rect (taking in count size of text) 
    painter->drawText((rsize.width() - size.width()) / 2, (rsize.height() + size.height() / 2) / 2, name);
    //go under rect center with offset
    painter->translate(0, rsize.height() + OFFSET);
    //paint stem with offset in mind
    QLine center_line(QPoint{ rsize.width() / 2, -OFFSET }, QPoint{ rsize.width() / 2,cont_height + OFFSET });
    painter->drawLine(center_line);

    for (auto act : activations) {
        int start = act.startIndex() * ACTION_RH;
        int end = qMax(act.endIndex(), size_t(start + 1)) * ACTION_RH;
        //draw activation rectangle
        painter->drawRect((rsize.width() - ACTIVATION_W) / 2, start, ACTIVATION_W, end);
    }
}

void Column::movePos(QPointF& pos)
{
    setPos(pos);
    pos = { pos.x() + rsize.width() + OFFSET, pos.y() };
}

void Column::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
}
