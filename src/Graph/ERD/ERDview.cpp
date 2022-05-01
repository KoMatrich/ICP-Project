#include "Graph/ERD/View.h"

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
        rem(items().count() - 1);
    }

    uint index = 0;
    //add or change existing items
    while (!classes.empty()) {
        UMLClass clas = classes.back();
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

        index++;
        classes.pop_back();
    }
}

void ERDScene::add(UMLClass const clas)
{
    WItem* item = new WItem{ this, clas };
    addItem(item);
}

void ERDScene::rem(uint index)
{
    //remove old from display
    items().removeAt(index);
    //deletes old
    delete items().at(index);
}

ERDView::ERDView(QObject* parent)
{
    setScene(&scene);
    setRenderHint(QPainter::Antialiasing);
    //TODO fix this background
    const QPixmap bckg = QPixmap(":/images/resources/grid.png");
    setBackgroundBrush(bckg);
    //setCacheMode(QGraphicsView::CacheBackground);//dont use! visual glitches
    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setInteractive(true);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setAlignment(Qt::AlignCenter);
}

void ERDView::update()
{
    scene.update();
}
