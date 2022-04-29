#include <Graph/ERDview.h>
#include <Graph/ERDItem.h>
#include "Semantics.h"

ERDScene::ERDScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);
}

void ERDScene::update()
{
    auto sem = Semantics::getInstance();
    auto classes = sem->getClasses();

    int offset = 0;

    //remove all excessive items
    while (items().count() > classes.size()) {
        items().removeLast();
    }

    uint index = 0;
    //add or change existing items
    while (!classes.empty()) {
        auto clas = classes.back();
        if (index >= items().count()) {
            //item doesn't exists
            //new item
            add(clas);
        } else if (clas.has_changed) {
            //item exists and has changed
            rem(index);
            //create new
            add(clas);
        }//else nothing

        classes.pop_back();
    }
}

void ERDScene::add(UMLClass const clas)
{
    WItem* item = new WItem{ clas };
    addItem(item);
}

void ERDScene::rem(uint index)
{
    //remove old from display
    items().removeAt(index);
    //deletes old
    delete items().at(index);
}

void ERDScene::documentWasModified()
{
    update();
}

ERDView::ERDView(QObject* parent)
{
    setScene(&scene);
    setRenderHint(QPainter::Antialiasing);
    //TODO fix this background
    QPixmap bckg = QPixmap(":images/res/grid.png");
    setBackgroundBrush(bckg);
    setCacheMode(QGraphicsView::CacheBackground);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    setInteractive(true);
    setDragMode(QGraphicsView::ScrollHandDrag);
}
