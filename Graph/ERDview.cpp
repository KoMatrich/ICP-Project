#include <QtWidgets>
#include "ERDview.h"
#include "DragItem.h"

ERDview::ERDview(QTextEdit *parent) : QWidget(parent)
{
    this->editor = parent;
    resize(200, 200);
    setAcceptDrops(true);
}

void ERDview::documentWasModified()
{

}

void ERDview::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("app/ERDview")) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }

    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void ERDview::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("app/ERDview")) {
        if (children().contains(event->source())) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else if (event->mimeData()->hasText()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void ERDview::dropEvent(QDropEvent *event)
{
    //accept only drops from this QWidget
    if (event->mimeData()->hasFormat("app/ERDview")) {
        const QMimeData *mime = event->mimeData();

        QByteArray itemData = mime->data("app/ERDview");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QString text;
        QPoint offset;
        dataStream >> text >> offset;

        DragLabel *newLabel = new DragLabel(text, this);
        newLabel->move(event->pos() - offset);
        newLabel->show();
        newLabel->setAttribute(Qt::WA_DeleteOnClose);

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    }
}

void ERDview::mousePressEvent(QMouseEvent *event)
{
    DragLabel *child = static_cast<DragLabel*>(childAt(event->pos()));
    if (!child)
        return;

    QPoint hotSpot = event->pos() - child->pos();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << child->labelText() << QPoint(hotSpot);

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("app/ERDview", itemData);
    mimeData->setText(child->labelText());

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(*child->pixmap());
    drag->setHotSpot(hotSpot);

    child->hide();

    if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
        child->close();
    else
        child->show();
}
