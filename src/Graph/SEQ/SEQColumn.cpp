#include "Graph/SEQ/SEQColumn.h"

SEQColumn::SEQColumn(QGraphicsScene* parent, QPoint pos, QPoint& off, SEQMember& mem, const int& height)
	: cont_height(height), name(mem.getName())
{
	setAcceptHoverEvents(true);

	//calculate size
	auto w = QFontMetrics(QApplication::font()).boundingRect(name).width();
	size = { w, HEADER_HEIGHT };
	rsize = QSize{ w, HEADER_HEIGHT } + SOFFSET;

	line = mem.getLine();
    c_line = mem.getClassLine();

	//create infill
	if (has_error = mem.getErrorFlag()) {
		fill = redG(size.height());
	} else {
		if (is_interface = mem.isInterface()) {
			fill = blueG(size.height());
		} else {
			fill = greenG(size.height());
		}
	}

	//get all activations
	activations = mem.getActivations();
	//update pos
	setPos(pos);
	off = { rsize.width() / 2, 0 };
}

QRectF SEQColumn::boundingRect() const
{
	return QRectF(0, 0,
				  rsize.width(),
				  HEADER_SPACE + HEADER_HEIGHT + OFFSET + cont_height * (ACTION_RH + ACTION_S) + STEM_EXTRA
	).normalized().marginsAdded(BOUND_OF);
}

void SEQColumn::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	QPen thick_pen = QPen();
	thick_pen.setWidth(2);

	//draw background frame
	painter->setRenderHint(QPainter::Antialiasing);
	painter->setBrush(fill);
	if (is_thick) {
		painter->setPen(thick_pen);
	}
	painter->translate(rsize.width() / 2.0, HEADER_SPACE + rsize.height());
	painter->save();
	//go to header start
	painter->translate(0, -HEADER_SPACE);
	//draw stem with extra down
	painter->drawLine(0, 0, 0, HEADER_SPACE + cont_height * (ACTION_RH + ACTION_S) + STEM_EXTRA);
	//go to top left corner of header
	painter->translate(-rsize.width() / 2.0, -rsize.height());
	//draw rect
	painter->drawRoundedRect(0, 0, rsize.width(), rsize.height(), RADIUS / 10, RADIUS / 10);
	//go to center of rect
	painter->translate(rsize.width() / 2.0, rsize.height() / 2.0);
	//draw text (centered)
	auto s = QFontMetrics(QApplication::font()).boundingRect(name);
	QPointF textOffset{ -size.width() / 2.0, s.height() / 2.0 };
	painter->drawText(textOffset, name);

	painter->restore();
	//paint down from center
	for (auto& act : activations) {
		//draw at start pos
		int start = act.startIndex() * (ACTION_RH + ACTION_S);
		//draw end of activation with litle tail
		int end = act.endIndex() * (ACTION_RH + ACTION_S) + (ACTION_RH / 8.0);
		//draw activation rectangle
		painter->drawRect(-ACTIVATION_W / 2.0, start, ACTIVATION_W, end - start);
	}

	drawDebug(painter, this);
}

void SEQColumn::movePos(QPointF& pos)
{
	setPos(pos);
	pos = { pos.x() + rsize.width() + OFFSET, pos.y() };
}

void SEQColumn::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
	//menu creation
	QMenu menu{};
    if (has_error) {
        menu.addAction(QStringLiteral("Add class..."), [this]() {CodeService::addEntity(name, false); });
        menu.addAction(QStringLiteral("Add interface..."), [this]() {CodeService::addEntity(name, true); });
        menu.addSeparator();
    }
    else {
        if (is_interface) {
            menu.addAction(QStringLiteral("View interface"), [this]() {CodeService::highlightLine(c_line); });
        }
        else {
            menu.addAction(QStringLiteral("View class"), [this]() {CodeService::highlightLine(c_line); });
        }
    }

	menu.addAction(QStringLiteral("Modify"), [this]() {CodeService::highlightLine(line); });

	//menu execution
	menu.exec(event->screenPos());

	//relations is pointer to local menu data
	//no need to delete pointer data
}

void SEQColumn::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
	is_thick = true;
	update();
}

void SEQColumn::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
	is_thick = false;
	update();
}
