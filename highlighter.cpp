#include "highlighter.h"
#include <QDebug>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    err.setBackground(Qt::red);
    after_err.setForeground(Qt::darkGray);

    Rule r;
    r.start = QRegExp("@startuml");
    r.end   = QRegExp("@enduml");
    r.format.setFontWeight(QFont::Bold);

    syntax.append(r);
}

int Highlighter::match(const QString &text, int &offset, RuleSet &parts, Rule &current)
{
    //find start of sub block
    for(int i = 0; i < parts.length();i++){
        Rule part = parts.at(i);
        QRegExp start = part.start;
        int match_i = start.indexIn(text,offset);
        if(match_i == offset){
            setFormat(offset,start.matchedLength(),part.format);
            offset += start.matchedLength();
            setCurrentBlockState(1);
            stack.append(i);
            return 0;
        }
    }

    //find end of actual block
    QRegExp end = current.end;
    if(!end.isEmpty()){
        int match_i = end.indexIn(text,offset);
        if(match_i == offset){
            setFormat(offset,end.matchedLength(),current.format);
            offset += end.matchedLength();
            setCurrentBlockState(1);
            stack.pop_back();
            return 1;
        }
    }

    //nothing has been found
    setFormat(offset,text.length(),err);
    return -1;
}

void Highlighter::find(const QString &text, int &offset)
{
    if(syntax.length()==0){
        setCurrentBlockState(-3);
        return;
    }

    //empty line check
    if(text.isEmpty()){
        setCurrentBlockState(1);
        return;
    }

    RuleSet parts = syntax;
    Rule current;
    if(stack.length()==0){
        //stack empty       => not in body
    }else{
        //stack not empty   => in body
        int stack_l = stack.length();
        int i = 0;
        while(stack_l > i){
            current = parts.at(stack.at(i));
            parts   = current.parts;
            i++;
        }
    }

    match(text,offset,parts,current);
}

void Highlighter::highlightBlock(const QString &text)
{
    setCurrentBlockState(-2);
    switch(previousBlockState()){
    case -3:
        //internal error
        setCurrentBlockState(-3);
        break;
    case -2:
        //error ocured in syntax last check
        setFormat(0,text.length(),after_err);
        setCurrentBlockState(-2);
        break;
    case -1:
        //start of file
        stack.clear();
    case 1:
        //normal operation
        int start = 0;
        find(text,start);
        break;
    }
}
