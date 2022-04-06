#include "Highlighter.h"

Highlighter::Highlighter(QTextEdit *parent)
    : QSyntaxHighlighter(parent->document())
{
    err.setBackground(Qt::red);
    parent->setTextColor(Qt::black);
    after_err.setForeground(Qt::darkGray);
    cursor_color.setBackground(Qt::yellow);

    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    parent->setFont(font);

    const int tabStop = 3;  //characters
    QFontMetrics metrics(font);
    parent->setTabStopWidth(tabStop * metrics.width(' '));
}

void Highlighter::getRules(Rule &current,RuleSet &parts, Path *path){
    parts = syntax; //default body of code

    if(path->length()!=0){
        //path not empty   => in body
        int i = 0;
        while(path->length() > i){
            current = parts.at(path->at(i));
            parts   = current.parts;
            i++;
        }

        if(current.end.isEmpty()){
            //one line block without end
            path->pop_back();
            getRules(current,parts,path);
        }
    }
}

int Highlighter::match(const QString &text, int &offset, Path *path)
{
    RuleSet parts;
    Rule current;
    getRules(current,parts,path);

    //find end of current block
    QRegExp end = current.end;
    if(!end.isEmpty()){
        int match_i = end.indexIn(text,offset);
        if(match_i == offset){
            setFormat(offset,end.matchedLength(),current.format);
            offset += end.matchedLength();
            return -1;
        }
    }

    //find start of sub block
    for(int i = 0; i < parts.length();i++){
        Rule part = parts.at(i);
        QRegExp start = part.start;
        int match_i = start.indexIn(text,offset);
        if(match_i == offset){
            setFormat(offset,start.matchedLength(),part.format);
            offset += start.matchedLength();
            return i;
        }
    }
    //no rule was matched
    return -2;
}

void Highlighter::find(const QString &text, int line, int &offset)
{
    while(line < path_stack.length()){
        //line number bigger than actual stack index
        //needs to remove overhead
        path_stack.pop_back();
    }

    //path to current block
    Path *path;
    if(line == 0){
        //first line
        path = new Path();
    }else{
        //load stack from previous line
        if(path_stack.length()<line){
            error(-3,"Stack error index out of bounds");
            return;
        }
        path = new Path(path_stack.at(line-1));
    }

    int result = match(text,offset,path);

    switch(result){
    default:
        path->append(result);
        break;
    case -1:
        path->pop_back();
        break;
    case -2:
        if(path->empty()){
            //outside of uml body
            //ignore all
        }else{
            //wrong input
            setCurrentBlockState(-2);
            setFormat(offset,text.length(),err);
        }
        break;
    }
    path_stack.append(*path);
}

void inline Highlighter::error(int code, const QString msg){
    qDebug() << msg;
    setCurrentBlockState(code);
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

    if(prevState < -1)
        switch(prevState){
        case INTERNAL_E:
            setCurrentBlockState(INTERNAL_E);
            break;
        case SYNTAX_E:
            setFormat(0,text.length(),after_err);
            setCurrentBlockState(SYNTAX_E);
            break;
        }
    else
        switch(prevState){
        case FILE_START:
            path_stack.clear();
        default:
            //normal operation
            if(text.isEmpty()){
                //empty line copy previous number
                int state = prevState;
                setCurrentBlockState(state);
                break;
            }else{
                //update line number
                updateCurrentBlockState();
            }

            int lineNumber = currentBlockState()/2;
            int offset = 0;
            //run syntax check
            find(text,lineNumber,offset);
            break;
        }
}
