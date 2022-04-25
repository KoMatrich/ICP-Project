#include <Graph/DragItem.h>

#include <QtWidgets>

DragItem::DragItem(const QString &text, QWidget *parent)
    : QLabel(parent),itemData(text)
{
    QSize frameSize{ itemData.Width() + OFFSET * 2, itemData.Height() + OFFSET * 2 };

    QImage image(frameSize, QImage::Format_ARGB32_Premultiplied);
    image.fill(qRgba(0, 0, 0, 0));

    QFont font;
    font.setStyleStrategy(QFont::ForceOutline);

    QLinearGradient gradient(0, 0, 0, image.height()-1);
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.2, QColor(220, 255, 220));
    gradient.setColorAt(0.8, QColor(220, 240, 220));
    gradient.setColorAt(1.0, QColor(220, 240, 220));

    QPainter painter;
    painter.begin(&image);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(gradient);
    painter.drawRoundedRect(QRectF(QPoint(0,0), frameSize),
                            RADIUS, RADIUS, Qt::RelativeSize);

    painter.setFont(font);
    painter.setBrush(Qt::black);
    PaintBlocks(painter, itemData);
    painter.end();

    setPixmap(QPixmap::fromImage(image));
}


QString DragItem::labelText() const
{
    return itemData.Text();
}

void DragItem::PaintBlocks(QPainter& paint, ItemData data) {
    paint.translate(OFFSET, OFFSET);
    for (ItemDL line : data.Lines()) {
        //magic needed
        // TODO: rewrite!
        if (line.type == ItemDLT::Separator) {
            addLineSplit(paint, line, data);
            continue;
        } else {
            paint.drawText(QRect(0,0,data.Width(),data.Line_h()), Qt::AlignLeft, line.data);
            paint.translate(0, data.Line_h());
        }
    }
}

//draws line across block
void DragItem::addLineSplit(QPainter& paint, ItemDL line, ItemData data)
{

    const auto space = 2;
    QPoint points[]{
        QPoint(-OFFSET,0), QPoint(itemData.Width() + OFFSET,0),
        QPoint(itemData.Width() + OFFSET, -space), QPoint(-OFFSET, -space)
    };

    QChar c = *line.data.data();
    switch (c.toLatin1()) {
    case '-':
        paint.drawLine(points[0], points[1]);
        break;
    case '_':
        paint.drawPolygon(points, 4);
        break;
    case '=':
        paint.drawLine(points[0], points[1]);
        paint.drawLine(points[2], points[3]);
        break;
    default:
        qDebug("Unknown selector (PaintBlocks)");
        break;
    }
    paint.translate(0, data.Separator_h());
}
