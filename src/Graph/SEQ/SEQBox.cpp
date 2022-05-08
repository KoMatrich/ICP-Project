#include "Graph/SEQ/SEQBox.h"

SEQBox::SEQBox(QGraphicsScene* parent, QRect rect_t, QString name) :rect(rect_t), name(name)
{
    setPos(rect.x(), rect.y());
    this->rect.setX(0);
    this->rect.setY(0);
    this->rect = this->rect.marginsAdded(BOX_OFF);
}

QRectF SEQBox::boundingRect() const
{
    return rect.normalized().marginsAdded(BOX_OFF).marginsAdded(SEQ_BOUND_OF);
}

void SEQBox::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    painter->save();
    painter->translate(-BOX_OFFSET, 0);
    int time_index = 0;
    for (int height = HEADER_HEIGHT + HEADER_SPACE + BOX_OFFSET; height < rect.height() - STEM_EXTRA; height += ACTION_RH + ACTION_S) {
        painter->drawLine(0, height, TIME_LINE_W, height);

        QString index = QString::number(time_index++);
        painter->drawText(pixOf, height - pixOf, index);
    }
    painter->restore();

    //draw bounds
    painter->drawRect(rect);
    //go to top center of bounds
    painter->translate(rect.width() / 2, 0);
    //offset to draw centered
    QRectF descRect = QFontMetrics(QApplication::font()).boundingRect(name);
    painter->translate(-descRect.width() / 2, -descRect.height() / 2);
    descRect += {pixOf, 0, pixOf, 0};

    painter->setBrush(QColor(225, 255, 225));
    painter->drawRect(descRect);
    painter->drawText(0, 0, name);

    drawDebug(painter, this);
}
