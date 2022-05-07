#include "Graph/SEQ/Column.h"

Column::Column(QGraphicsScene* parent, QPoint pos, QPoint& off, SEQMember& mem, const int& height)
    : cont_height(height), name(mem.getName())
{
    //calculate size
    auto w = QFontMetrics(QApplication::font()).boundingRect(name).width();
    size = { w, HEADER_HEIGHT };
    rsize = QSize{ w, HEADER_HEIGHT } + SOFFSET;

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
    setPos(pos);
    off = { rsize.width()/2, 0 };
}

QRectF Column::boundingRect() const
{
    return QRectF(0, 0,
                  rsize.width(),
                  HEADER_SPACE + HEADER_HEIGHT + OFFSET + cont_height * ACTION_RH + STEM_EXTRA
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
    painter->translate(rsize.width() / 2.0, HEADER_SPACE + rsize.height());
    painter->save();
    //go to header start
    painter->translate(0, -HEADER_SPACE);
    //draw stem with extra down
    painter->drawLine(0, 0, 0, HEADER_SPACE + cont_height * ACTION_RH + STEM_EXTRA);
    //go to top left corner of header
    painter->translate(-rsize.width() / 2.0, -rsize.height());
    //draw rect
    painter->drawRoundedRect(0, 0, rsize.width(), rsize.height(), RADIUS / 10, RADIUS / 10);
    //go to center of rect
    painter->translate(rsize.width() / 2.0, rsize.height() / 2.0);
    //draw text (centered)
    auto s = QFontMetrics(QApplication::font()).boundingRect(name);
    QPointF textOffset{ -size.width() / 2.0, s.height() / 2.0 };
    painter->drawText(textOffset, name);

    painter->restore();
    //paint down from center
    for (auto& act : activations) {
        //draw at start pos
        int start = act.startIndex() * ACTION_RH;
        //draw end of activation with litle tail
        int end = act.endIndex() * ACTION_RH + (ACTION_RH / 8.0);
        //draw activation rectangle
        painter->drawRect(-ACTIVATION_W / 2.0, start, ACTIVATION_W, end - start);
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
