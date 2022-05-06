#include "Graph/SEQ/Column.h"

Column::Column(QGraphicsScene* parent, QPointF& offsetPos, SEQMember& mem, const int& height, const int& COLUMN_SPACING)
    : cont_height(height), name(mem.getName())
{
    //calculate size
    size = { metric.width(name),metric.height() };
    rsize = QSize{ size.width(), size.height() } + SOFFSET;

    //create infill
    if (mem.getErrorFlag()) {
        fill = redG(size.height());
    } else {
        if (mem.isInterface()) {
            fill = blueG(size.height());
        } else {
            fill = greenG(size.height());
        }
    }

    //get all activations
    activations = mem.getActivations();
    //update pos
    setPos(offsetPos);
    offsetPos += QPoint(COLUMN_SPACING, 0);
}

QRectF Column::boundingRect() const
{
    return QRectF(0, 0, rsize.width(), rsize.height() + cont_height * ACTION_RH);
}

void Column::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    drawDebug(painter, this);

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
    painter->save();
    //paint up from center
    painter->translate(0, -HEADER_SPACE);
    //draw stem
    painter->drawLine(0, 0, 0, HEADER_SPACE + (cont_height + 1) * ACTION_RH);
    painter->translate(-rsize.width() / 2, -rsize.height());
    //draw rect
    painter->drawRoundedRect(0, 0, rsize.width(), rsize.height(), RADIUS / 10, RADIUS / 10);
    painter->translate(rsize.width() / 2, rsize.height() / 2);
    //draw text in rectangle (centered)
    QPoint textOffset{ -metric.width(name) / 2, metric.height() / 2 };
    painter->drawText(textOffset, name);

    painter->restore();
    //paint down from center
    for (auto& act : activations) {
        int start = act.startIndex() * ACTION_RH;
        int end = act.endIndex() * ACTION_RH + (ACTION_RH / 4);
        //draw activation rectangle
        painter->drawRect(-ACTIVATION_W / 2, start, ACTIVATION_W, end - start);
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
