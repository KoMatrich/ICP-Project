#include <Graph/ERDItem.h>

WItem::WItem(UMLClass clas)
{
    addline({ BlockType::Text, clas.getClassName() });
    addline({ BlockType::SepDouble ,"" });
    auto atrs = clas.getAttributes();
    for (auto atr : atrs) {
        addline({ BlockType::Text,atr.toString() });
    }

    addline({ BlockType::SepSingle ,"" });
    auto mets = clas.getMethods();
    for (auto met : mets) {
        addline({ BlockType::Text,met.toString() });
    }

    setPos({ qreal(clas.getXPos()),qreal(clas.getYPos()) });

    //"constant" init //QGraphicsWidget 
    size = Size();
    rsize = RSize();

    if (clas.getErrorFlag()) {
        fill = red();
    } else {
        fill = green();
    }

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
}

//adds new data to item
void WItem::addline(const Block line)
{
    auto text = metric.size(Qt::TextLongestVariant, line.data.trimmed());
    blocks.append(line);
    switch (line.type) {
    case BlockType::Text:
        width = qMax(width, text.width());
        line_h = qMax(line_h, text.height());
        line_c++;
        break;
    case BlockType::SepSingle:
    case BlockType::SepBold:
    case BlockType::SepDouble:
        separator_c++;
        break;
    default:
        break;
    }
}

QRectF WItem::boundingRect() const
{
    return { QPoint(0,0), rsize };
}

void WItem::paint(QPainter* painter,
                  const QStyleOptionGraphicsItem* option,
                  QWidget* widget)
{
    //draw background frame
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(fill);
    painter->drawRoundedRect(boundingRect(), RADIUS, RADIUS);

    //draw text in frame
    painter->translate(0, OFFSET);
    PaintBlocks(painter);
}

QVariant WItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange && scene()) {
        //snap to grid
        QPointF newPos = value.toPointF();
        if (QApplication::mouseButtons() == Qt::LeftButton) {
            qreal x = round(newPos.x() / GRID_S) * GRID_S;
            qreal y = round(newPos.y() / GRID_S) * GRID_S;
            return QPointF(x, y);
        } else
            return newPos;
    } else
        return QGraphicsItem::itemChange(change, value);
}

//paints block of item
void WItem::PaintBlocks(QPainter* paint)
{
    for (Block line : blocks) {
        if (line.type != BlockType::Text) {
            paintSeparator(paint, line.type);
        } else {
            paintText(paint, line);
        }
    }
}

//paints text block
void WItem::paintText(QPainter* paint, Block line)
{
    paint->drawText(QRect(OFFSET, 0, width + OFFSET, line_h), Qt::AlignLeft, line.data);
    paint->translate(0, line_h);
}

//paints separator block
void WItem::paintSeparator(QPainter* paint, BlockType type)
{
    switch (type) {
    case BlockType::SepSingle:
        paint->drawLine(points()[0], points()[1]);
        break;
    case BlockType::SepBold:
        paint->drawPolygon(points(), 4);
        break;
    case BlockType::SepDouble:
        paint->drawLine(points()[0], points()[1]);
        paint->drawLine(points()[2], points()[3]);
        break;
    default:
        qDebug("Unknown selector (PaintBlocks)");
        break;
    }
    paint->translate(0, SEPARATOR_H);
}

QLinearGradient WItem::green()
{
    QLinearGradient gradient(0, 0, 0, RSize().height());
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.2, { 220, 255, 220 });
    gradient.setColorAt(0.8, { 220, 240, 220 });
    gradient.setColorAt(1.0, { 220, 240, 220 });
    return gradient;
}

QLinearGradient WItem::red()
{
    QLinearGradient gradient(0, 0, 0, RSize().height());
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.2, { 255, 220, 220 });
    gradient.setColorAt(0.8, { 240, 220, 220 });
    gradient.setColorAt(1.0, { 240, 220, 220 });
    return gradient;
}

constexpr QPoint* WItem::points()
{
    QPoint points[]{
        QPoint(0,0),
        QPoint(rsize.width(),0),
        QPoint(rsize.width(),SEPARATOR_H),
        QPoint(0,SEPARATOR_H)
    };

    for (auto point : points) {
        point -= POFFSET;
    }
    return points;
}
