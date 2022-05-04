#include <Graph/ERD/Item.h>

WItem::WItem(QGraphicsScene* parent, UMLClass clas)
{
    this->setParent(parent);

    addline({ BlockType::Text, clas.getClassName() });
    addline({ BlockType::SepBold ,"" });
    addAtributes(clas);
    addline({ BlockType::SepDouble ,"" });
    addMethods(clas);
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

void WItem::addAtributes(UMLClass clas)
{
    //attributes
    auto atrs = clas.getAttributes();
    for (auto atr : atrs) {
        addline({ BlockType::Text,atr.toString() });
    }

    //inhered attributes
    auto iatrs = clas.getInheritedAttributes();
    if (atrs.size() > 0, iatrs.size() > 0)
        addline({ BlockType::SepSingle ,"" });
    for (auto iatr : iatrs) {
        addline({ BlockType::Text,iatr.toString() });
    }
}

void WItem::addMethods(UMLClass clas)
{
    //methods
    auto meths = clas.getMethods();
    for (auto meth : meths) {
        addline({ BlockType::Text,meth.toString() });
    }

    //inheret methods
    auto imeths = clas.getInheritedMethods();
    if (meths.size() > 0, imeths.size() > 0)
        addline({ BlockType::SepSingle ,"" });
    for (auto imeth : imeths) {
        addline({ BlockType::Text,imeth.toString() });
    }
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
    return QRectF{ QPoint(0,0), rsize }.normalized(); //.marginsAdded(MARGIN)
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
            emit itemMoved();
            return QPointF(x, y);
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void WItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    //menu creation
    QMenu menu{};
    menu.addAction(QStringLiteral("Modify"), [this]() {});
    menu.addAction(QStringLiteral("Delete"), [this]() {});
    menu.addSeparator();
    QMenu* relations = menu.addMenu(QStringLiteral("Add relation"));
    relations->addAction(QStringLiteral("Aggregation"), [this]() {});
    relations->addAction(QStringLiteral("Association"), [this]() {});
    relations->addAction(QStringLiteral("Composition"), [this]() {});
    relations->addAction(QStringLiteral("Generalization"), [this]() {});

    //menu execution
    menu.exec(event->screenPos());

    //relations is pointer to local menu data
    //no need to delete pointer data
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
    QPoint points[]{
        QPoint(0,0),
        QPoint(rsize.width(),0),
        QPoint(rsize.width(),SEPARATOR_H),
        QPoint(0,SEPARATOR_H)
    };

    for (auto point : points) {
        point -= POFFSET;
    }

    QPainterPath path;
    switch (type) {
    case BlockType::SepSingle:
        paint->drawLine(points[0], points[1]);
        break;
    case BlockType::SepBold:
        path.moveTo(points[0]);
        path.lineTo(points[1]);
        path.lineTo(points[2]);
        path.lineTo(points[3]);
        paint->fillPath(path, QBrush(QColor("black")));
        break;
    case BlockType::SepDouble:
        paint->drawLine(points[0], points[1]);
        paint->drawLine(points[2], points[3]);
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
