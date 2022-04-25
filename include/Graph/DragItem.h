#ifndef DRAGLABEL_H
#define DRAGLABEL_H

#include <QLabel>
#include <Graph/DragItemPrecomp.h>

QT_BEGIN_NAMESPACE
class QDragEnterEvent;
class QDragMoveEvent;
class QFrame;
QT_END_NAMESPACE

class DragItem : public QLabel
{
public:
    DragItem(const QString &text, QWidget *parent);
    QString labelText() const;

private:
    const int OFFSET = 5;
    const int RADIUS = 30;

    QSize size;

    void PaintBlocks(QPainter& paint, ItemData data);
    void addLine(QPainter &paint, QString text);
    void addLineSplit(QPainter& paint, ItemDL line, ItemData type);

    ItemData itemData;
};

#endif // DRAGLABEL_H
