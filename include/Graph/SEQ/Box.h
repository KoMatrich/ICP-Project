#pragma once

#include "Graph/ItemConst.h"

class Box : public QGraphicsObject
{
public:
    /// @brief          inicializes box
    /// @param parent   graphics scene
    /// @param rect_t   size and pos
    /// @param name     name
    Box(QGraphicsScene* parent, QRect rect_t, QString name);
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
private:
    /// @brief          size of box
    QRect rect;
    /// @brief          name of box
    const QString name;
};
