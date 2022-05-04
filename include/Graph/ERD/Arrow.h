#pragma once

#include "Graph/Item.h"
#include "Syntax/Rules.h"

/// @brief QGraphicsObject Arrow
class Arrow : public QGraphicsObject
{
    Q_OBJECT
public:
    /// @brief              Creates new arrow
    /// @param parent       QGraphicsScene to which is this item linked
    /// @param o1           first target (to which is this arrow pointing)
    /// @param o2           second target (from which is this arrow pointing)
    /// @param arr_type     type of arrow head used
    Arrow(QGraphicsScene* parent, QGraphicsObject* o1, QGraphicsObject* o2, RuleID arr_type);
    /// @brief              override of virtual function
    /// @return             bounding box of arrow
    QRectF boundingRect() const override;
    /// @brief              override of virtual function
    /// @param painter      scene painter
    /// @param option       graphicsItem options
    /// @param widget       widget to draw to
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;

private:
    /// @brief Relative vector from first to second target
    QVector2D end;
    /// @brief Relative vector from first to second target boundary
    QVector2D col_vec;
    /// @brief first target
    QGraphicsObject* o1;
    /// @brief second target
    QGraphicsObject* o2;
    /// @brief updates end and col_vec
    void updateArrow();

    /// @brief Arrow polygon
    QPolygonF arrow_head;
    /// @brief Arrow type
    RuleID arrow_type;
    /// @brief updates arrow_head depending on arrow_type and col_vec
    void updateArrowHead();
private slots:
    /// @brief calls update functions
    void update();
    /// @brief calls destructor of this object
    void destroy();
};
