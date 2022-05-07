#include "Graph/SEQ/View.h"

#include "Graph/SEQ/SEQColumn.h"
#include "Graph/SEQ/SEQArrow.h"
#include "Graph/SEQ/SEQBox.h"
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

    QPoint start{ 0,0 };
    QPoint last{ 0,0 };

    for (auto seq : seqs) {
        //get data
        std::vector<SEQMember> members = seq.getMembers();
        std::vector<SEQAction> acts = seq.getActions();

        //calculate max spacing width
        int COLUMN_SPACING = 0;
        for (auto& act : acts) {
            COLUMN_SPACING = qMax(COLUMN_SPACING, metric.width(act.getMethod()) + OFFSET * 2);
        }
        COLUMN_SPACING += COLUMN_SPACE;

        //keep track of stem starts
        std::vector<QPoint> pos;
        std::vector<QPoint> offs;
        QPoint off;

        //create headers with bodies
        for (auto& member : members) {
            pos.push_back(start);
            addColumn(member, start, off, acts.size());
            start += off;
            last = start;
            start += {COLUMN_SPACING, 0};

            offs.push_back(off);
        }

        //create actions
        uint timeIndex = 0;
        for (auto& act : acts) {
            addArrow(act, timeIndex, pos, offs);
        }

        start = pos.front();

        last += {COLUMN_SPACING, HEADER_HEIGHT + HEADER_SPACE + int(acts.size()) * (ACTION_RH + ACTION_S) + STEM_EXTRA + BOX_OFFSET};
        last += off;

        SEQBox* box = new SEQBox(this, QRect(start, last - start), seq.getName());
        box->ItemStacksBehindParent;
        addItem(box);

        start.setX(last.x());
        start += {BOX_OFFSET * 4, 0};
    }
}

void SEQScene::addColumn(SEQMember& member, QPoint pos, QPoint& off, const int& height)
{
    auto col = new SEQColumn(this, pos, off, member, height);
    addItem(col);
}

void SEQScene::addArrow(SEQAction& action, uint& timeIndex, std::vector<QPoint> pos, std::vector<QPoint> off)
{
    QPoint offset{ 0, int(HEADER_HEIGHT + HEADER_SPACE + POFFSET.x() + timeIndex * (ACTION_RH + ACTION_S)) };

    timeIndex++;

    if (action.getType() == RuleID::R_NOP || action.getErrorLevel() >= 2) {
        return;
    }

    int startI = action.getSenderIndex();
    int endI = action.getReceiverIndex();

    QPoint start = pos.at(startI) + off.at(startI) + offset;
    QPoint end = pos.at(endI) + off.at(endI) + offset;

    auto arr = new SEQArrow(this, start, end, action.getType(), action.getMethod(), action.getErrorLevel(), action.getLine(), action.getClassEndIndex());
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
}
