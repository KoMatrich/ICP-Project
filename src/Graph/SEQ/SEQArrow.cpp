#include "Graph/SEQ/SEQArrow.h"
#include "Services/Code.h"

SEQArrow::SEQArrow(QGraphicsScene* parent, const QPoint& pos1, const QPoint& pos2, const RuleID& arr_type, const QString& method, const int error, size_t ln, size_t c_ln)
    :pos1(pos1), pos2(pos2), method(method), has_error(error), line(ln), classLine(c_ln)
{
    setAcceptHoverEvents(true);
    arrow_type = arr_type;
    setParent(parent);

    update();
}

QRectF SEQArrow::boundingRect() const
{
    if (pos1 != pos2) {
        return QRect(0, -ACTION_H / 2, col_vec.toPoint().x(), ACTION_RH).normalized().marginsAdded(SEQ_BOUND_OF);
    } else {
        return QRect(0, -ACTION_H / 2, SELF_ARROW_WIDTH + metric.width(method), ACTION_RH).normalized().marginsAdded(SEQ_BOUND_OF);
    }
}

void SEQArrow::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    QPen pen = QPen();

    if (is_thick) {
        pen.setWidth(2);
    }

    if (has_error) {
        pen.setColor(Qt::red);
    } else {
        pen.setColor(Qt::black);
    }

    if (arrow_type == RuleID::R_ARROW_ASYNC)
        pen.setStyle(Qt::DashLine);
    else
        pen.setStyle(Qt::SolidLine);

    QBrush brush = QBrush(Qt::white);

    painter->setPen(pen);
    painter->setBrush(brush);

    QPoint textOffset{ metric.width(method) / 2, metric.height() / 2 };

    // if not self referencing:
    if (pos1 != pos2) {
        painter->drawLine(end.toPoint(), col_vec.toPoint());
    }
    // otherwise, create an 'U' Shape
    else {
        painter->drawLine(end.toPoint(), end.toPoint() + QPointF(SELF_ARROW_WIDTH, 0));
        painter->drawLine(end.toPoint() + QPointF(SELF_ARROW_WIDTH, 0), end.toPoint() + QPointF(SELF_ARROW_WIDTH, SELF_ARROW_HEIGHT));
        painter->drawLine(end.toPoint() + QPointF(SELF_ARROW_WIDTH, SELF_ARROW_HEIGHT), end.toPoint() + QPointF(0, SELF_ARROW_HEIGHT));
    }

    painter->drawPolygon(arrow_head);
    painter->setFont(QApplication::font());

    if (abs(pos1.x() - pos2.x()) > 0) {
        painter->translate((pos1 - pos2) / 2 - textOffset);
    } else {
        painter->translate(QPoint{ int(ARROW_GEN_SIZE),-metric.height() / 2 });
    }

    if (pos1 == pos2)
        painter->translate(ACTIVATION_W / 2, 0);

    auto m = QFontMetrics(QApplication::font()).boundingRect(method);
    QRect descRect{ 0,0,m.width(),m.height() };
    painter->drawRect(descRect.marginsAdded(SEQ_BOUND_OF));
    painter->drawText(QPoint{ 0,metric.height() }, method);

    drawDebug(painter, this);
}

void SEQArrow::update()
{
    updateArrow();
    updateArrowHead();
}

void SEQArrow::updateArrow()
{
    //set arrow position to center of object 
    setPos(pos2);

    //get connecting vector
    QVector2D end = QVector2D(pos1 - pos2);

    //get colision vector without arrow head size
    col_vec = QVector2D(end.x(), end.y());
}

void SEQArrow::updateArrowHead()
{
    double angle = std::atan2(-col_vec.y(), col_vec.x());

    QPointF offset;

    if (pos1 == pos2) {
        offset = QPointF(0, SELF_ARROW_HEIGHT);
    } else {
        offset = QPointF(0, 0);
    }

    QPointF arrowP1 = end.toPointF() + QPointF(sin(angle + M_PI / 3) * ARROW_SEQ_SIZE,
        cos(angle + M_PI / 3) * ARROW_SEQ_SIZE) + offset;

    QPointF arrowP2 = end.toPointF() + QPointF(sin(angle + 2 * M_PI / 3) * ARROW_SEQ_SIZE,
        cos(angle + 2 * M_PI / 3) * ARROW_SEQ_SIZE) + offset;

    arrow_head.clear();
    arrow_head << end.toPointF() + offset << arrowP1 << arrowP2;
}

void SEQArrow::destroy()
{
    delete this;
}

void SEQArrow::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    //menu creation
    QMenu menu{};
    if (has_error) {
        menu.addAction(QStringLiteral("Add declaration..."), [this]() {CodeService::insertLine(classLine, "\t+ void " + method + "\n"); });
        menu.addSeparator();
    }
    menu.addAction(QStringLiteral("Modify"), [this]() {CodeService::highlightLine(line); });
    menu.addAction(QStringLiteral("Delete"), [this]() {CodeService::deleteEntity(line, line); });
    menu.addAction(QStringLiteral("Duplicate"), [this]() {CodeService::duplicateLine(line); });

    //menu execution
    menu.exec(event->screenPos());

    //relations is pointer to local menu data
    //no need to delete pointer data
}
