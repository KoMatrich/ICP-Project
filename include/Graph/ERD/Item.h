/**
 * @file Item.h
 *
 * @brief Contains declaration of item for ERD
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

 /// @brief ERD scene item
class ERDItem : public QGraphicsObject
{
    Q_OBJECT
public:
    /// @brief          creates ERD item
    /// @param parent   graphics scene on which is displayed
    /// @param clas     class data
    ERDItem(QGraphicsScene* parent, UMLClass clas);
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
protected:
    /// @brief          overridden function (handles position change)
    /// @param change   change in item pos
    /// @param value    value of change
    /// @return         change
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant& value);
    /// @brief          context menu handler
    /// @param event    context menu open event
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
    /// @brief          updates code on mouse release
    /// @param event    mouse release event
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    /// @brief          marks the entity as selected while in connect mode
    /// @param event    mouse hover enter event
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    /// @brief          marks the entity as not selected while in connect mode
    /// @param event    mouse hover leave event
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    /// @brief          connects the relation (if any in connect mode)
    /// @param event    mouse press event
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    /// @brief          enter the connect mode
    /// @param rule     specifies the option selected
    void connectMode(RuleID rule);
private:
    //automatic vars
    //used for size calculations
    /// rectangle infill
    QLinearGradient fill;
    /// @brief width of rectangle
    int width{ 0 };
    /// @brief number of separators
    int separator_c{ 0 };
    /// @brief max height of text line
    int line_h{ 0 };
    /// @brief number of text lines
    int line_c{ 0 };

    /// @brief      calculates height of item
    /// @return     height of item
    int Height() { return line_c * line_h + separator_c * SEPARATOR_H; }

    /// @brief used for highlighting
    bool is_thick = false;
    /// @brief used for connecting to other classes
    bool is_selected = false;

    //feed back loop
    /// @brief used for getting position of cords in text file
    size_t xpos_line{ 0 };
    /// @brief used for getting position of cords in text file
    size_t ypos_line{ 0 };
    /// @brief used for getting position of cords in text file
    size_t class_line{ 0 };
    /// @brief used for getting position of cords in text file
    size_t class_end{ 0 };
    /// @brief used for getting position of cords in text file
    QString class_name;

    /// @brief cashed position for movement
    QPoint cached_pos;

    /// @brief  size of object
    /// @return QSize
    constexpr QSize Size() { return QSize{ this->width, this->Height() }; }
    /// @brief  real size for bounding box function
    QSize rsize;
    /// @brief  real size of object
    /// @return QSize
    constexpr QSize RSize()
    {
        return QSize{ qCeil(qreal(Size().width() + POFFSET.x() * 2) / GRID_S) * GRID_S,
        qCeil(qreal(Size().height() + POFFSET.x() * 2) / GRID_S) * GRID_S };
    }

    /// @brief data blocks (text,separators)
    QVector<Block> blocks;

    /// @brief init processing function for data
    void addline(const Block data);

    //paint sub functions
    /// @brief          paints all blocks
    /// @param paint    painter
    void PaintBlocks(QPainter* paint);
    /// @brief          paints specific text line
    /// @param paint    painter
    /// @param line     text line
    void paintText(QPainter* paint, Block line);
    /// @brief          paints separator
    /// @param paint    painter
    /// @param type     string representation of type
    void paintSeparator(QPainter* paint, BlockType type);

private:
    /// @brief          adds attributes to item
    /// @param clas     data
    void addAtributes(UMLClass clas);
    /// @brief          adds methods to item
    /// @param clas     data
    void addMethods(UMLClass clas);

signals:
    /// @brief  Signals item movement
    /// @note   Use queued signal connection only if using item pos
    void itemMoved();
private slots:
    /// @brief  Adapt to code change
    void changeCode();
};
