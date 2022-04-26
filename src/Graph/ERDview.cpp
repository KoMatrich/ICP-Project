#include <Graph/ERDview.h>
#include <Graph/ERDItem.h>

ERDview::ERDview(QObject* parent)
    : QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    ERDItem* test1 = new ERDItem();
    addItem(test1);
    test1->setPos(0, 0);

    ERDItem* test2 = new ERDItem();
    addItem(test2);
    test2->setPos(30, 30);
}

void ERDview::documentWasModified()
{

}
