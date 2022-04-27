#include <Graph/ERDItem.h>

WItem::WItem() :
    hash(0), name("")
{
    //TODO init
    addline({ BlockType::Text, "test"});
    addline({ BlockType::Separator, "--" });
    addline({ BlockType::Text, "long text test" });

    //"constant" init
    size = Size();
    rsize = RSize();
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
    case BlockType::Separator:
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
    QLinearGradient gradient(0, 0, 0, rsize.height());
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.2, { 220, 255, 220 });
    gradient.setColorAt(0.8, { 220, 240, 220 });
    gradient.setColorAt(1.0, { 220, 240, 220 });

    //draw background frame
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(gradient);
    painter->drawRoundedRect(boundingRect(), RADIUS, RADIUS);

    //draw text in frame
    painter->translate(0, OFFSET);
    PaintBlocks(painter);
}

//paints block of item
void WItem::PaintBlocks(QPainter* paint)
{
    for (Block line : blocks) {
        if (line.type == BlockType::Separator) {
            paintSeparator(paint, line);
        } else {
            paintText(paint, line);
        }
    }
}

//paints text block
void WItem::paintText(QPainter* paint, Block line)
{
    paint->drawText(QRect(OFFSET, 0, width, line_h), Qt::AlignLeft, line.data);
    paint->translate(0, line_h);
}

//paints separator block
void WItem::paintSeparator(QPainter* paint, Block line)
{
    QChar c = *line.data.data();
    switch (c.toLatin1()) {
    case '-':
        paint->drawLine(points()[0], points()[1]);
        break;
    case '_':
        paint->drawPolygon(points(), 4);
        break;
    case '=':
        paint->drawLine(points()[0], points()[1]);
        paint->drawLine(points()[2], points()[3]);
        break;
    default:
        qDebug("Unknown selector (PaintBlocks)");
        break;
    }
    paint->translate(0, SEPARATOR_H);
}

constexpr QPoint* WItem::points()
{
    QPoint points[]{
        QPoint(0,0),
        QPoint(rsize.width(),0),
        QPoint(rsize.width(),rsize.height()),
        QPoint(0,rsize.height())
    };

    for (auto point : points) {
        point -= POFFSET;
    }
    return points;
}