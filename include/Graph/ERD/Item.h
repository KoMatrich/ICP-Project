#pragma once

#include "Semantics.h"
#include "Graph/ItemConst.h"

class ERDItem : public QGraphicsObject
{
    Q_OBJECT
public:
    ERDItem(QGraphicsScene* parent, UMLClass clas);
    QRectF boundingRect() const override;
    void paint(QPainter* painter,
        const QStyleOptionGraphicsItem* option,
        QWidget* widget) override;
    QString getBlock(uint i) { return blocks.at(i).data; };
protected:
    QVariant itemChange(GraphicsItemChange change,
        const QVariant& value);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);
    //void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
private:
    //automatic var
    QLinearGradient fill;
    int width{ 0 };
    int separator_c{ 0 };
    int line_h{ 0 };
    int line_c{ 0 };

    int Height() { return line_c * line_h + separator_c * SEPARATOR_H; }
    int Width() { return width; }

    size_t xpos_line{ 0 };
    size_t ypos_line{ 0 };
    size_t class_line{ 0 };
    QSize size;
    QSize rsize;
    QPoint cached_pos;

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
    QLinearGradient red();
    QLinearGradient green();
    QLinearGradient blue();

private:
    void addAtributes(UMLClass clas);
    void addMethods(UMLClass clas);

    void test();

signals:
    void itemMoved();
private slots:
    void changeCode();
};
