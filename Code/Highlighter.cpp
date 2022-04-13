#include "Highlighter.h"

Highlighter::Highlighter(QTextEdit *parent)
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

    syntax = new SyntaxTree();
    analyzer = new Analyzer(syntax);
}

/// Line index numbering updating
/// chenges index from odd to even
/// and back, to make chenge in indexing.
/// Required to updated all remaining
/// lines after edited line.
int inline state_modify(int state){
    int index = state / 2;
    int mod = state % 2;
    return index*2 + (mod+1)%2;
}

/// Logic inplementation of line indexing.
void Highlighter::updateCurrentBlockState(){
    auto prevState = previousBlockState();

    if(prevState == -1){
        //first block
        int state;
        if(currentBlockState() == -1){
            //new block
            state = 0;
        }else{
            //just edit
            int dif = currentBlockState()/2 - prevState/2;
            if(dif==0)
                state = state_modify(currentBlockState());
            else
                state = state_modify(prevState+2);
        }
        setCurrentBlockState(state);
    }else{
        //not first
        int state;
        int dif = currentBlockState()/2 - prevState/2;
        if(dif==1){
            //just change
            state = state_modify(currentBlockState());
        }else{
            //line before was delete/removed/added
            state = state_modify(prevState+2);
        }
        setCurrentBlockState(state);
    }
}

/// Skips to next non white space char on line
void Highlighter::skipSpace(const QString text, int &offset){
    //save current offset
    int tmp = offset;
    //move offset to next non whitespace
    for(auto c: text.mid(tmp))
        if(c.isSpace())
            offset++;
        else{
            if(offset >= text.length())
                offset = text.length();
            return;
        }
    if(offset >= text.length())
        offset = text.length();
}

/// Main highlighter functions
void Highlighter::highlightBlock(const QString &text){
    auto prevState = previousBlockState();

    if(prevState < -1){
        //check if previous line failed
        switch(prevState){
        case INTERNAL_E:
            setCurrentBlockState(INTERNAL_E);
            break;
        case SYNTAX_E:
            setFormat(0,text.length(),syntax->after_err);
            setCurrentBlockState(SYNTAX_E);
            break;
        }
        return;
    }

    if(prevState == FILE_START){
        //start of file
        //reset analyzer
        analyzer->ClearAll();
    }

    //normal operation
    if(text.isEmpty()){
        //empty line copy previous number
        //dont do any syntax check
        int state = prevState;
        setCurrentBlockState(state);
        return;
    }

    //update line number
    updateCurrentBlockState();

    int lineNumber = currentBlockState()/2;
    int len = text.length();

    int offset = 0;
    int last_off = 0;

    //run syntax check
    //  clear syntax stack to current line
    analyzer->ClearTo(lineNumber);
    Rule rule;

    //  match everything possible on this line
    do{
        skipSpace(text, offset);

        last_off = offset;
        analyzer->Next(lineNumber,offset,text,rule);

        if(offset > 0)
            setFormat(last_off,offset-last_off,rule.format);

    }while((offset>=0)&&(offset<len));

    if(offset==len)
        return;

    switch(offset){
    case NO_CHECK:
        setFormat(last_off,len-last_off,syntax->no_check);
        break;
    case SYNTAX_ERR:
        setFormat(last_off,len-last_off,syntax->err);
        setCurrentBlockState(SYNTAX_E);
        break;
    case INTERNAL_E:
        setCurrentBlockState(INTERNAL_E);
        break;
    }
}
