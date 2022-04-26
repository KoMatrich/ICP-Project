#include "Graph/ERDItem.h"

ERDItem::ERDItem()
{
    size = QSize(30, 30);
}

QRectF ERDItem::boundingRect() const
{
    return QRectF(QPoint(0,0), size);
}

void ERDItem::paint(QPainter* painter,
                    const QStyleOptionGraphicsItem* option,
                    QWidget* widget)
{
    QLinearGradient gradient(0, 0, 0, size.height());
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.2, QColor(220, 255, 220));
    gradient.setColorAt(0.8, QColor(220, 240, 220));
    gradient.setColorAt(1.0, QColor(220, 240, 220));

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(gradient);
    painter->drawRoundedRect(boundingRect(), RADIUS, RADIUS);
}