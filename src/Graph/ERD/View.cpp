#include "Graph/ERD/View.h"
#include "Services/SSemantics.h"

ERDScene::ERDScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);
}

void ERDScene::update()
{
    Semantics sem = Semantics::getInstance();
    auto classes = sem.getClasses();

    clear();

    addClasses(classes);
    addArrows(classes);
}

void ERDScene::addClasses(std::vector<UMLClass> classes)
{
    //add or change existing items
    for (UMLClass clas : classes) {
        ERDItem* item = new ERDItem{ this, clas };
        item->installEventFilter(this);
        addItem(item);
    }
}

void ERDScene::addArrows(std::vector<UMLClass> classes)
{
    for (size_t i = 0; i < classes.size(); i++) {
        auto clas = classes.at(i);
        for (UMLRelation& rel : clas.getRelations()) {
            ERDItem* i1 = dynamic_cast<ERDItem*>(items().at(classes.size() - i - 1));
            ERDItem* i2 = dynamic_cast<ERDItem*>(items().at(classes.size() - rel.getID() - 1));

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

ERDView::ERDView(QObject* parent)
{
    setScene(&scene);
    setRenderHint(QPainter::Antialiasing);
    const QPixmap bckg = QPixmap(":/images/resources/grid.png");
    setBackgroundBrush(bckg);
    //setCacheMode(QGraphicsView::CacheBackground);//dont use! visual glitches
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setInteractive(true);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setAlignment(Qt::AlignCenter);
}
