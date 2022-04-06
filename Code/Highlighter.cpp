#include "Highlighter.h"

Highlighter::Highlighter(QTextEdit *parent)
    : QSyntaxHighlighter(parent->document())
{
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    parent->setFont(font);

    const int tabStop = 3;  //characters
    QFontMetrics metrics(font);
    parent->setTabStopWidth(tabStop * metrics.width(' '));

    syntax = new Syntax();
    analyzer = new Analyzer(syntax);
}

//  doc handeling
int inline state_modify(int state){
    int index = state / 2;
    int mod = state % 2;
    return index*2 + (mod+1)%2;
}

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
        //others
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

//handles line numbering and calls find funct
void Highlighter::highlightBlock(const QString &text){
    auto prevState = previousBlockState();

    if(prevState < -1){
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
    int last_off;

    //run syntax check
    analyzer->ClearTo(lineNumber);
    Rule rule;

    do{
        last_off = offset;
        analyzer->Next(lineNumber,offset,text,rule);

        if(offset > 0)
            setFormat(last_off,offset-last_off,rule.format);
    }while((offset>=0)&&(offset<len));

    if(offset==len){
        return;
    }else{
        offset = SYNTAX_ERR;
    }

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
