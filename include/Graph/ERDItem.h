#pragma once

#include "qgraphicsitem.h"
#include "Semantics.h"

//item data line type
enum class BlockType
{
    Text, SepSingle, SepBold, SepDouble
};

//item data line
class Block
{
public:
    Block(BlockType type, QString data)
    {
        this->type = type;
        this->data = data;
    }

    BlockType type;
    QString data;
};

class WItem : public QGraphicsItem
{
public:
    WItem(UMLClass clas);
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget) override;
protected:
    QVariant itemChange(GraphicsItemChange change,
                        const QVariant& value);

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
    //used by paintSeparator
    constexpr QPoint* points();

    //data
    QVector<Block> blocks;

    QFontMetrics const metric = QFontMetrics{ QApplication::font() };

    //init processing function
    void addline(const Block data);
    //paint sub functions
    void PaintBlocks(QPainter* paint);
    void paintText(QPainter* paint, Block line);
    void paintSeparator(QPainter* paint, BlockType type);

    QLinearGradient green();
    QLinearGradient red();
};
