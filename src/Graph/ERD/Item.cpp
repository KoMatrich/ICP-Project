#include <Graph/ERD/Item.h>

ERDItem::ERDItem(QGraphicsScene* parent, UMLClass clas)
{
    this->setParent(parent);

    xpos_line = clas.getXLine();
    ypos_line = clas.getYLine();
    class_line = clas.pos;
    class_end = clas.pos_end;
    class_name = clas.getClassName();

    addline({ BlockType::Text, class_name });
    addline({ BlockType::SepBold ,"" });
    addAtributes(clas);
    addline({ BlockType::SepDouble ,"" });
    addMethods(clas);
    moveBy(qreal(clas.getXPos()), qreal(clas.getYPos()));

    rsize = RSize();

    cached_pos = QPoint(clas.getXPos(), clas.getYPos());

    if (clas.getErrorFlag()) {
        fill = redG(rsize.height());
    } else {
        if (clas.isInterface())
            fill = blueG(rsize.height());
        else
            fill = greenG(rsize.height());
    }

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemClipsToShape, true);
    setFlag(QGraphicsItem::ItemSendsScenePositionChanges, true);
    setAcceptHoverEvents(true);
    //setFlag(QGraphicsItem::ItemIgnoresTransformations, true);
}

void ERDItem::addAtributes(UMLClass clas)
{
    //attributes
    auto atrs = clas.getAttributes();
    for (UMLProperty& atr : atrs) {
        addline({ BlockType::Text,atr.toString() });
    }

    //inhered attributes
    auto iatrs = clas.getInheritedAttributes();
    if (atrs.size() > 0, iatrs.size() > 0)
        addline({ BlockType::SepSingle ,"" });
    for (UMLProperty& iatr : iatrs) {
        addline({ BlockType::InheritedText,iatr.toString() });
    }
}

void ERDItem::addMethods(UMLClass clas)
{
    //methods
    auto meths = clas.getMethods();
    for (UMLProperty& meth : meths) {
        addline({ BlockType::Text,meth.toString() });
    }

    //inheret methods
    auto imeths = clas.getInheritedMethods();
    if (meths.size() > 0, imeths.size() > 0)
        addline({ BlockType::SepSingle ,"" });
    for (UMLProperty& imeth : imeths) {
        addline({ BlockType::InheritedText,imeth.toString() });
    }
}

//adds new data to item
void ERDItem::addline(const Block line)
{
    auto text = QFontMetrics(QApplication::font()).boundingRect(line.data.trimmed());

    blocks.append(line);
    switch (line.type) {
    case BlockType::InheritedText:
        [[FALLTHROUGH]];
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

QRectF ERDItem::boundingRect() const
{
    return QRectF{ QPoint(0,0), rsize }.normalized().marginsAdded(BOUND_OF);
}

void ERDItem::paint(QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
    QPen thick_pen = QPen();
    thick_pen.setWidth(3);
    QPen selected_pen = QPen();
    selected_pen.setWidth(5);
    selected_pen.setColor(Qt::red);

    //draw background frame
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(fill);
    if (is_selected) {
        painter->setPen(selected_pen);
    } else if (is_thick) {
        painter->setPen(thick_pen);
    }
    painter->drawRoundedRect(boundingRect(), RADIUS, RADIUS);

    //draw text in frame
    painter->translate(0, OFFSET);
    painter->setFont(QApplication::font());
    PaintBlocks(painter);

    drawDebug(painter, this);
}

QVariant ERDItem::itemChange(GraphicsItemChange change, const QVariant& value)
{
    if (change == ItemPositionChange && scene()) {
        //snap to grid
        QPointF newPos = value.toPointF();
        if (QApplication::mouseButtons() == Qt::LeftButton) {
            qreal x = cached_pos.x() + round(newPos.x() / GRID_S) * GRID_S;
            qreal y = cached_pos.y() + round(newPos.y() / GRID_S) * GRID_S;

            emit itemMoved();
            CodeService::setPosActive(true);
            CodeService::cacheUpdatePos(class_line, xpos_line, static_cast<int>(x), ypos_line, static_cast<int>(y));

            return QPointF(x, y);
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void ERDItem::changeCode()
{
    CodeService::updatePos(class_line, xpos_line, static_cast<int>(this->pos().x()), ypos_line, static_cast<int>(this->pos().y()));
}

void ERDItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    //menu creation
    QMenu menu{};
    menu.addAction(QStringLiteral("Modify"), [this]() {CodeService::highlightLine(class_line); });
    menu.addAction(QStringLiteral("Delete"), [this]() {CodeService::deleteEntity(class_line, class_end); });
    menu.addSeparator();
    QMenu* relations = menu.addMenu(QStringLiteral("Add relation"));
    relations->addAction(QStringLiteral("Aggregation"), [this]() {connectMode(RuleID::R_AGG); });
    relations->addAction(QStringLiteral("Association"), [this]() {connectMode(RuleID::R_ASS); });
    relations->addAction(QStringLiteral("Composition"), [this]() {connectMode(RuleID::R_COM); });
    relations->addAction(QStringLiteral("Generalization"), [this]() {connectMode(RuleID::R_GEN); });

    //menu execution
    menu.exec(event->screenPos());

    //relations is pointer to local menu data
    //no need to delete pointer data
}

void ERDItem::connectMode(RuleID rule)
{
    is_selected = true;
    CodeService::relationRequest(class_name, rule, class_end);
    update();
}

void ERDItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    CodeService::callCachedUpdatePos();
}

void ERDItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    is_thick = true;
    update();
}

void ERDItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    is_thick = false;
    update();
}

void ERDItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    CodeService::relationConnect(class_name);
}

//paints block of item
void ERDItem::PaintBlocks(QPainter* paint)
{
    for (Block line : blocks) {
        switch (line.type) {
        case BlockType::InheritedText:
            paintText(paint, line);
            break;
        case BlockType::Text:
            paintText(paint, line);
            break;
        default:
            paintSeparator(paint, line.type);
            break;
        }
    }
}

//paints text block
void ERDItem::paintText(QPainter* paint, Block line)
{
    if (line.type == BlockType::Text) {
        paint->setPen(Qt::black);
    } else {
        paint->setPen(Qt::gray);
    }
    paint->drawText(QRect(OFFSET, 0, width + OFFSET, line_h), Qt::AlignLeft, line.data);
    paint->translate(0, line_h);
}

//paints separator block
void ERDItem::paintSeparator(QPainter* paint, BlockType type)
{
    paint->setPen(Qt::black);
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
