#include <Graph/ERDview.h>
#include <Graph/ERDItem.h>

ERDview::ERDview(QObject* parent)
    : QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);

    WItem* test1 = new WItem();
    addItem(test1);
    test1->setPos(0, 0);

    WItem* test2 = new WItem();
    addItem(test2);
    test2->setPos(30, 30);


}

void ERDview::update()
{
    QList<WItem*> inputList;

    //create backup of items to iterate over them safely
    auto itemList = items();

    for(auto itemL : itemList) {
        auto LItem = static_cast<WItem*>(itemL);
        bool found = false;

        //match loop
        //faster thanks for list shortening
        for (auto IItem : inputList) {
            if (LItem->name != IItem->name)
                continue;

            //item has same name
            if (LItem->hash != IItem->hash) {
                //current element is not updated
                //remove from display
                removeItem(LItem);
            } else {
                //current element is updated
                //remove from input items
                inputList.removeAt(inputList.indexOf(IItem));
            }
            //found something leave match loop
            break;
        }

        if (!found) {
            //item is not in input
            //remove item
            removeItem(LItem);
        }
    }

    //create new items that are missing
    for (auto IItem : inputList) {
        addItem(IItem);
    }
}

void ERDview::documentWasModified()
{
    update();
}
