/**
 * @file SEQArrow.h
 *
 * @brief Contains declaration of arrow for SEQ
 *
 * @author Martin Kocich
 * Contact: xkocic02@stud.fit.vutbr.cz
 *
 * @author Vitezslav Kriz
 * Contact xkrizv03@stud.fit.vutbr.cz
 *
 */

#pragma once

#include "Graph/ItemConst.h"
#include "Syntax/Rules.h"

 /// @brief QGraphicsObject Arrow
class SEQArrow : public QGraphicsObject
{
    Q_OBJECT
public:

    /// @brief              Creates new arrow
    /// @param parent       QGraphicsScene to which is this item linked
    /// @param pos1         first target pos (to which is this arrow pointing)
    /// @param pos2         second target pos (from which is this arrow pointing)
    /// @param arr_type     type of arrow head used
    /// @param method       name of arrow action
    /// @param error        error level
    /// @param ln           action line
    /// @param c_ln         class line
    /// @param m_ln         method line
    SEQArrow(QGraphicsScene* parent, const QPoint& pos1, const QPoint& pos2, const RuleID& arr_type, const QString& method, const int error, size_t ln, size_t c_ln, size_t m_ln);
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
protected:
    /// @brief          context menu handler
    /// @param event    context menu open event
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
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
    const int error_level;
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
    /// @brief is being hovered over?
    bool is_thick = false;
    /// @brief line in code
    size_t line = 0;
    /// @brief last line of class for inserting
    size_t classLine = 0;
    /// @brief last line of class for inserting
    size_t methodLine = 0;
private slots:
    /// @brief calls destructor of this object
    void destroy();
};
