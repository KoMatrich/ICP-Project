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

    clear();

    //add or change existing items
    for (auto clas : classes) {
        add(clas);
    }

    //TEST Delete me
    if (items().count() >= 2) {
        WItem* i1 = dynamic_cast<WItem*>(items().at(0));
        WItem* i2 = dynamic_cast<WItem*>(items().at(1));
        if (i1 != nullptr && i2 != nullptr) {
            auto arrow = new Arrow(this, i1, i2, RuleID::R_AGG);

            arrow->setFlag(QGraphicsItem::ItemStacksBehindParent);
            addItem(arrow);
        } else {
            VitaPrint("Arrow");
        }
    }
}

void ERDScene::add(UMLClass const data)
{
    WItem* item = new WItem{ this, data };
    addItem(item);
}

ERDView::ERDView(QObject* parent)
{
    setScene(&scene);
    setRenderHint(QPainter::Antialiasing);
    //TODO fix this background
    const QPixmap bckg = QPixmap(":/images/resources/grid.png");
    setBackgroundBrush(bckg);
    //setCacheMode(QGraphicsView::CacheBackground);//dont use! visual glitches
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setInteractive(true);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setAlignment(Qt::AlignCenter);
}
