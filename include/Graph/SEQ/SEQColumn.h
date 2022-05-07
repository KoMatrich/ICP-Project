/**
 * @file SEQColumn.h
 *
 * @brief Contains declaration of column for SEQ
 *
 * @author Martin Kocich
 * Contact: xkocic02@stud.fit.vutbr.cz
 *
 * @author Vitezslav Kriz
 * Contact xkrizv03@stud.fit.vutbr.cz
 *
 */

#pragma once

#include "Semantics.h"
#include "Graph/ItemConst.h"

 /// @brief SEQ scene column
class SEQColumn : public QGraphicsObject
{
    Q_OBJECT
public:
    /// @brief          creates ERD item
    /// @param parent   graphics scene on which is displayed
    /// @param pos      position of right top corner
    /// @param off      this offset
    /// @param mem      SEQMember to get name from
    /// @param height   height of column content (count)
    SEQColumn(QGraphicsScene* parent, QPoint pos, QPoint& off, SEQMember& mem, const int& height);
    /// @brief          calculates bounding box
    /// @return         bounding box of this item
    QRectF boundingRect() const override;
    /// @brief          paint function of this item
    /// @param painter  scene painter 
    /// @param option   object painting option
    /// @param widget   widget on which is painted
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    /// @brief          moves column to pos
    /// @param pos      absolute pos
    void movePos(QPointF& pos);
protected:
    /// @brief          context menu handler
    /// @param event    context menu open event
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
    /// @brief          marks the entity as selected while in connect mode
    /// @param event    mouse event
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    /// @brief          marks the entity as not selected while in connect mode
    /// @param event    mouse event
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
private:
    /// @brief height of content (count)
    const int cont_height;
    /// @brief name of column
    QString name;

    /// @brief all activation blocks
    std::vector<SEQActivation> activations;

    /// @brief  size of text
    QSize size;
    /// @brief  top rectangle size
    QSize rsize;
    /// @brief flag for hovering effect
    bool is_thick = false;
    /// @brief saved filling gradient for background drawing
    QLinearGradient fill;
    /// @brief line in code
    size_t line;
    /// @brief Class line in code
    size_t c_line;
    /// @brief error flag
    bool has_error = false;
    /// @brief interface flag
    bool is_interface = false;
};

