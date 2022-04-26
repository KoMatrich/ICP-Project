#include "qgraphicsitem.h"

class ERDItem: public QGraphicsItem
{
public:
	ERDItem();
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

private:
    const int OFFSET = 5;
    const int RADIUS = 10;

    QSize size;
};
