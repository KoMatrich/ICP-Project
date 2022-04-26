#pragma once

#include <QWidget>
#include <Graph/DragItem.h>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class ERDview : public QWidget
{
    Q_OBJECT
public:
    explicit ERDview(QWidget *parent = 0);
    QGraphicsView* View(){return view;}

public slots:
    void documentWasModified();
private:
    QGraphicsScene *scene;
    QGraphicsView* view;

    //TODO fix this
    QPixmap bckg = QPixmap(":images/res/grid.png");
};
