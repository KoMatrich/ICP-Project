#pragma once

#include "Semantics.h"
#include "Services/Code.h"
#include "Graph/Item.h"

class WItem : public QGraphicsObject
{
    Q_OBJECT
public:
    WItem(QGraphicsScene* parent, UMLClass clas);
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
    QString getBlock(uint i) { return blocks.at(i).data; };
protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant& value);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

private:
    //automatic var
    QLinearGradient fill;
    int width{ 0 };
    int separator_c{ 0 };
    int line_h{ 0 };
    int line_c{ 0 };

    int Height() { return line_c * line_h + separator_c * SEPARATOR_H; }
    int Width() { return width; }

    QSize size;
    QSize rsize;

    constexpr QSize Size() { return QSize{ this->Width(), this->Height() }; }
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

signals:
    void itemMoved();
};
