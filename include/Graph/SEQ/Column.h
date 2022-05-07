#pragma once

#include "Semantics.h"
#include "Graph/ItemConst.h"

/// @brief SEQ scene column
class Column : public QGraphicsObject
{
    Q_OBJECT
public:
    /// @brief          creates ERD item
    /// @param parent   graphics scene on which is displayed
    /// @param clas     class data
    Column(QGraphicsScene* parent, QPoint pos, QPoint& off, SEQMember& mem, const int& height);
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
    const int cont_height;
    QString name;

    std::vector<SEQActivation> activations;

    /// @brief  size of text
    QSize size;
    /// @brief  top rectangle size
    QSize rsize;
    /// @brief flag for hovering effect
    bool is_thick = false;
    /// @brief saved filling gradient for background drawing
    QLinearGradient fill;
    /// @brief line of code
    size_t line;
};

