#include <Graph/ERDview.h>
#include <Graph/ERDItem.h>

ERDview::ERDview(QWidget *parent)
{
    resize(200, 200);
    
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    view = new QGraphicsView(scene);
    view->setRenderHint(QPainter::Antialiasing);
    view->setBackgroundBrush(bckg);
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
    view->setInteractive(true);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->show();
}

void ERDview::documentWasModified()
{
    ERDItem *test1 = new ERDItem();
    scene->addItem(test1);
    test1->setPos(0, 0);

    ERDItem* test2 = new ERDItem();
    scene->addItem(test2);
    test2->setPos(30, 30);
}