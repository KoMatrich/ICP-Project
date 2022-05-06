#include "Graph/SEQ/View.h"

#include "Graph/SEQ/Column.h"
#include "Graph/SEQ/Arrow.h"
#include "Graph/SEQ/Box.h"
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
    QPoint start{ 0,0 };
    for (auto seq : seqs) {
        std::vector<SEQMember> members = seq.getMembers();
        std::vector<SEQAction> acts = seq.getActions();

        int COLUMN_SPACING = 0;
        for (auto& act : acts) {
            COLUMN_SPACING = qMax(COLUMN_SPACING, metric.width(act.getMethod()));
        }
        COLUMN_SPACING += COLUMN_SPACE;

        //keeps starting positions of stems with offset
        std::vector<QPoint> offsets;
        for (auto& member : members) {
            offsets.push_back(start);
            addColumn(member, start, acts.size(), COLUMN_SPACING);
        }

        uint timeIndex = 0;
        for (auto& act : acts) {
            addArrow(act, timeIndex, offsets);
        }

        Box* box = new Box(this, QRect(0, 0, start.x(), 0));
        addItem(box);
    }
}

void SEQScene::addColumn(SEQMember& member, QPoint& offsetPos, const int& height, const int& COLUMN_SPACING)
{
    auto col = new Column(this, offsetPos, member, height, COLUMN_SPACING);
    addItem(col);
}

void SEQScene::addArrow(SEQAction& action, uint& timeIndex, std::vector<QPoint> offsets)
{
    QPoint offset{ 0, int(HEADER_HEIGHT + HEADER_SPACE + POFFSET.x() + timeIndex * ACTION_RH) };

    timeIndex++;

    if (action.getType() == RuleID::R_NOP || action.getErrorLevel() >= 2) {
        return;
    }

    int startI = action.getSenderIndex();
    int endI = action.getReceiverIndex();

    QPoint start = offsets.at(startI) + offset;
    QPoint end = offsets.at(endI) + offset;

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
