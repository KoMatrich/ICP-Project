#pragma once

#include <QWidget>
#include <Graph/DragItem.h>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class ERDview : public QGraphicsScene
{
    Q_OBJECT
public:
    enum Mode { InsertItem, InsertLine, InsertText, MoveItem };

    explicit ERDview(QObject *parent = nullptr);

public slots:
    void documentWasModified();
};
