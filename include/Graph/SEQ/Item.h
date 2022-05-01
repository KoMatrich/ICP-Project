#pragma once

#include "qgraphicsitem.h"
#include "Semantics.h"
#include "qobject.h"
#include "Services/Code.h"
#include "Graph/Item.h"

class SeqItem : public QGraphicsObject
{
    Q_OBJECT
public:
    SeqItem(QGraphicsScene* parent, UMLClass clas);
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant& value);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

private:
    const int GRID_S = 12;
    const int OFFSET{ GRID_S / 2 };
    const int RADIUS{ GRID_S };
    const int SEPARATOR_H{ 3 };

    const QPoint POFFSET{ OFFSET,OFFSET };

    //automatic var
    QLinearGradient fill;
    int width{ 0 };
    int separator_c{ 0 };
    int line_h{ 0 };
    int line_c{ 0 };

    int Height() { return line_c * line_h + separator_c * SEPARATOR_H; }
    int Width() { return width; }

    QSize size;
    constexpr QSize Size() { return QSize{ this->Width(), this->Height() }; }
    QSize rsize;
    constexpr QSize RSize() { return QSize{ qCeil(qreal(size.width() + POFFSET.x() * 2) / GRID_S) * GRID_S, qCeil(qreal(size.height() + POFFSET.x() * 2) / GRID_S) * GRID_S }; }

    //data
    QVector<Block> blocks;

    QFontMetrics const metric = QFontMetrics{ QApplication::font() };

    //init processing function for data
    void addline(const Block data);

    //paint sub functions
    void PaintBlocks(QPainter* paint);
    void paintText(QPainter* paint, Block line);
    void paintSeparator(QPainter* paint, BlockType type);

    //for gradient fill
    QLinearGradient green();
    QLinearGradient red();

private:
    void addAtributes(UMLClass clas);
    void addMethods(UMLClass clas);

    void test();
};
