#include "Syntax/Highlighter.h"
#include "Services/Debug.h"
#include "Services/Code.h"
#include "Services/SSemantics.h"
#include "Services/History.h"

Highlighter::Highlighter(QTextEdit* parent)
    : QSyntaxHighlighter(parent->document())
{
    //setup basic highlight
    QFont font;
    font.setFamily("Monospace");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    parent->setFont(font);

    const int tabStop = 3;  //characters
    QFontMetrics metrics(font);
    parent->setTabStopWidth(tabStop * metrics.width(' '));

    syntax_tree = new SyntaxTree();
    analyzer = new Analyzer(syntax_tree);
}

/// Line index numbering updating
/// changes index from odd to even
/// and back, to make change in indexing.
/// Required to update all remaining
/// lines after edited line.
int inline state_modify(int state)
{
    int index = state / 2;
    int mod = state % 2;
    return index * 2 + (mod + 1) % 2;
}

void Highlighter::updateCurrentBlockState()
{
    auto prevState = previousBlockState();

    if (prevState == -1) {
        //first block
        int state;
        if (currentBlockState() == -1) {
            //new block
            state = 0;
        } else {
            //just edit
            int dif = currentBlockState() / 2 - prevState / 2;
            if (dif == 0)
                state = state_modify(currentBlockState());
            else
                state = state_modify(prevState + 2);
        }
        setCurrentBlockState(state);
    } else {
        //not first
        int state;
        int dif = currentBlockState() / 2 - prevState / 2;
        if (dif == 1) {
            //just change
            state = state_modify(currentBlockState());
        } else {
            //line before was delete/removed/added
            state = state_modify(prevState + 2);
        }
        setCurrentBlockState(state);
    }
}

void Highlighter::skipSpace(const QString text, int& offset)
{
    //save current offset
    int tmp = offset;
    //get all chars behind offset
    for (auto c : text.mid(tmp))
        if (c.isSpace())
            offset++;
        else {
            return;
        }
    //trim offset out of bounds
    if (offset >= text.length())
        offset = text.length();
}

void Highlighter::highlightBlock(const QString& text)
{
    if (!HighlightService::getEnabled()) return;

    std::string test = text.toStdString();
    auto prevState = previousBlockState();

    if (prevState < -1) {
        //check if previous line failed
        switch (prevState) {
        case INTERNAL_E:
            setCurrentBlockState(INTERNAL_E);
            break;
        case SYNTAX_E:
            setFormat(0, text.length(), syntax_tree->after_err);
            setCurrentBlockState(SYNTAX_E);
            break;
        }
        return;
    }

    if (prevState == FILE_START) {
        //start of file
        //reset analyzer
        analyzer->ClearAll();
        Semantics::getInstance().buildSTree(analyzer->GetStack());
    }

    //update line number
    updateCurrentBlockState();

    int lineNumber = currentBlockState() / 2;
    int len = text.length();

    int offset = 0;
    int last_off = 0;

    //run syntax check
    //  clear syntax stack to current line
    analyzer->ClearTo(lineNumber);

    Rule* rule = nullptr;

    //  match everything possible on this line
    do {
        skipSpace(text, offset);

        last_off = offset;
        analyzer->Next(lineNumber, offset, text, &rule);

        if (offset > 0)
            setFormat(last_off, offset - last_off, rule->format);

    }
    while ((offset >= 0) && (offset < len));

    switch (offset) {
    case NO_CHECK:
        VitaClear();
        VitaPrint("Nothing to compile!");
        setFormat(last_off, len - last_off, syntax_tree->no_check);
        return;
    case SYNTAX_ERR:
        VitaClear();
        VitaPrint("Syntax error!");
        CodeService::clearBackground();
        setFormat(last_off, len - last_off, syntax_tree->err);
        setCurrentBlockState(SYNTAX_E);
        return;
    case INTERNAL_E:
        VitaClear();
        VitaPrint("Internal error!");
        setCurrentBlockState(INTERNAL_E);
        return;
    }


    if (offset == len) {
        // syntax check OK -> pass tree for semantic check
        //call a singleton Semantics generator
        if (analyzer->GetStack().size() > 0)
            if (analyzer->GetStack().back().size() == 0) {
                //whole file has right syntax
                VitaClear();
                CodeService::clearBackground();
                Semantics::getInstance().buildSTree(analyzer->GetStack());
            }
        return;
    }
}
