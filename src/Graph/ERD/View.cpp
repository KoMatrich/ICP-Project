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

    for (size_t i = 0; i < classes.size(); i++) {
        for (auto rel : classes[i].getRelations()) {
            WItem* i1 = dynamic_cast<WItem*>(items().at(classes.size() - i - 1));
            WItem* i2 = dynamic_cast<WItem*>(items().at(classes.size() - rel.getID() - 1));

            if (i1 == nullptr || i2 == nullptr)
                continue;

            Arrow* arrow;

            if (rel.getType() == RuleID::R_GEN)
                arrow = new Arrow(this, i1, i2, rel.getType());
            else
                arrow = new Arrow(this, i2, i1, rel.getType());

            addItem(arrow);
        }
    }
}

void ERDScene::add(UMLClass const data)
{
    WItem* item = new WItem{ this, data };
    item->installEventFilter(this);
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
