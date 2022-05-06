#include "Graph/ItemConst.h"

void drawDebug(QPainter* painter, QGraphicsObject* obj)
{
#ifdef DEBUG_DRAW
    static int index = 0;
    index %= 255;

    painter->restore();
    painter->save();
    QPen debugDrawPen;
    debugDrawPen.setColor(QColor(255 - index, 0, index));
    debugDrawPen.setStyle(Qt::DashLine);
    debugDrawPen.setWidth(1);
    painter->setPen(debugDrawPen);
    painter->drawRect(obj->boundingRect());
    painter->drawEllipse(-3, -3, 6, 6);
    painter->restore();

    index += 50;
#endif // DEBUG_DRAW
}
