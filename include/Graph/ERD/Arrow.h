#pragma once

#include "qobject.h"
#include "Graph/ERD/Item.h"
#include "Graph/Item.h"
#include "Syntax/Rules.h"


class Arrow : public QGraphicsObject
{
    Q_OBJECT
public:
    Arrow(QGraphicsScene* parent, WItem* o1, WItem* o2, RuleID arr_type);

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

private:
    QVector2D end;
    QVector2D col_vec;
    QPolygonF arrow_head;
    RuleID arrow_type;
    //pointers to targets
    WItem* o1;
    WItem* o2;

    void updateArrow();
    void updateArrowHead();
private slots:
    void update();
    void destroy();
};
