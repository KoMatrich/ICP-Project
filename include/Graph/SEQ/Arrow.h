#pragma once

#include "Graph/ItemConst.h"
#include "Syntax/Rules.h"
#include "CMath.h"

/// @brief QGraphicsObject Arrow
class SEQArrow : public QGraphicsObject
{
    Q_OBJECT
public:
    /// @brief              Creates new arrow
    /// @param parent       QGraphicsScene to which is this item linked
    /// @param o1           first target (to which is this arrow pointing)
    /// @param o2           second target (from which is this arrow pointing)
    /// @param arr_type     type of arrow head used
    /// @param error        error level
    SEQArrow(QGraphicsScene* parent, const QPoint& pos1, const QPoint& pos2, const RuleID& arr_type, const QString& method, const int error);
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
    const QPoint pos1;
    /// @brief second target
    const QPoint pos2;

    /// @brief method that is displayed above arrow
    const QString method;
    /// @brief error level flag
    const int has_error;
    /// @brief updates end and col_vec
    void updateArrow();

    /// @brief Arrow polygon
    QPolygonF arrow_head;
    /// @brief Arrow type
    RuleID arrow_type;
    /// @brief updates arrow_head depending on arrow_type and col_vec
    void updateArrowHead();
    /// @brief calls update functions
    void update();
private slots:
    /// @brief calls destructor of this object
    void destroy();
};
