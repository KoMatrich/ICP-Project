#include "Graph/SEQ/View.h"
#include "Services/SSemantics.h"

SEQScene::SEQScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);
}

void SEQScene::update()
{
    Semantics sem = Semantics::getInstance();
    auto test = new Column(this, { 0,0 }, "Test", 100);
}

void SEQScene::addColumn(std::vector<UMLClass> classes)
{

}

SEQView::SEQView(QObject* parent)
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
