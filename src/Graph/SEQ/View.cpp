#include "Graph/SEQ/View.h"
#include "Graph/SEQ/Arrow.h"
#include "Services/SSemantics.h"

SEQScene::SEQScene(QObject* parent)
    : QGraphicsScene(parent)
{
    setItemIndexMethod(QGraphicsScene::BspTreeIndex);
}

void SEQScene::update()
{
    Semantics sem = Semantics::getInstance();
    auto seqs = sem.getSequences();

    //delete all
    clear();

    //check size
    if (seqs.size() == 0)
        return;

    //get data
    Sequence seq = seqs.at(0);
    std::vector<SEQMember> members = seq.getMembers();
    std::vector<SEQAction> acts = seq.getActions();

    //keeps starting positions of stems with offset
    std::vector<int> offsets;
    QPointF start{ 0,0 };
    for (auto& member : members) {
        offsets.push_back(start.x());
        addColumn(member, start, acts.size());
    }

    uint timeIndex = 0;
    for (auto& act : acts) {
        addArrow(act, timeIndex, offsets);
    }
}

void SEQScene::addColumn(SEQMember& member, QPointF& offsetPos, const int& height)
{
    auto col = new Column(this, offsetPos, member, height);
    addItem(col);
}

void SEQScene::addArrow(SEQAction& action, uint& timeIndex, std::vector<int> offsets)
{
    uint HOffset = timeIndex * ACTION_RH;

    timeIndex++;

    if (action.getType() == RuleID::R_NOP || action.getErrorLevel() >= 2) {
        return;
    }

    int startI = action.getSenderIndex();
    int endI = action.getReceiverIndex();

    QPoint start(offsets.at(startI), HOffset);
    QPoint end(offsets.at(endI), HOffset);

    auto arr = new SEQArrow(this, start, end, action.getType(), action.getMethod(), action.getErrorLevel());
    addItem(arr);
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

    QFont f = font();
    f.setPointSize(ACTION_H * 3 / 4);
    setFont(f);
}
