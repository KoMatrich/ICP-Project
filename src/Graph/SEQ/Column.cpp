#include "Graph/SEQ/Column.h"

Column::Column(QGraphicsScene* parent, QPoint& offsetPos, SEQMember& mem, const int& height, const int& COLUMN_SPACING)
    : cont_height(height), name(mem.getName())
{
    //calculate size
    size = { metric.width(name),HEADER_HEIGHT };
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
    setPos(offsetPos - QPoint{rsize.width()/2,0});
    offsetPos = { offsetPos.x() + COLUMN_SPACING, offsetPos.y() };
}

QRectF Column::boundingRect() const
{
    return QRectF(0, 0,
                  rsize.width(),
                  HEADER_SPACE + rsize.height() + cont_height * ACTION_RH + STEM_EXTRA
    ).normalized().marginsAdded(BOUND_OF);
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
    painter->translate(rsize.width() / 2, HEADER_SPACE + rsize.height());
    painter->save();
    //go to header start
    painter->translate(0, -HEADER_SPACE);
    //draw stem with extra down
    painter->drawLine(0, 0, 0, HEADER_SPACE + cont_height * ACTION_RH + STEM_EXTRA);
    //go to top left corner of header
    painter->translate(-rsize.width() / 2, -rsize.height());
    //draw rect
    painter->drawRoundedRect(0, 0, rsize.width(), rsize.height(), RADIUS / 10, RADIUS / 10);
    //go to center of rect
    painter->translate(rsize.width() / 2, rsize.height() / 2);
    //draw text (centered)
    QPoint textOffset{ -metric.width(name) / 2, metric.height() / 2 };
    painter->drawText(textOffset, name);

    painter->restore();
    //paint down from center
    for (auto& act : activations) {
        //draw at start pos
        int start = act.startIndex() * ACTION_RH;
        //draw end of activation with litle tail
        int end = act.endIndex() * ACTION_RH + (ACTION_RH / 8);
        //draw activation rectangle
        painter->drawRect(-ACTIVATION_W / 2, start, ACTIVATION_W, end - start);
    }

    drawDebug(painter, this);
}

void Column::movePos(QPointF& pos)
{
    setPos(pos);
    pos = { pos.x() + rsize.width() + OFFSET, pos.y() };
}

void Column::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
}
