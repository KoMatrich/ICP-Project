#include "highlighter.h"
#include <QDebug>

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    err.setBackground(Qt::red);
    after_err.setForeground(Qt::darkGray);

    //rule for main body
    Rule main;
    main.start = QRegExp("@startuml");
    main.end   = QRegExp("@enduml");
    main.format.setFontWeight(QFont::Bold);

    syntax.append(main);
}

int Highlighter::match(const QString &text, int &offset, RuleSet &parts, Rule &current, Path *path)
{
    //find start of sub block
    for(int i = 0; i < parts.length();i++){
        Rule part = parts.at(i);
        QRegExp start = part.start;
        int match_i = start.indexIn(text,offset);
        if(match_i == offset){
            setFormat(offset,start.matchedLength(),part.format);
            offset += start.matchedLength();
            path->append(i);
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
            path->pop_back();
            return 1;
        }
    }
    //no rule was matched
    return -1;
}

void Highlighter::find(const QString &text, int line)
{
    if(syntax.length()==0){
        setCurrentBlockState(-3);
        return;
    }

    Path *path;
    if(line == 0){
        //first line
        path = new Path();
    }else{
        //load stack from previous line
        if(path_stack.length()<line){
            qDebug() << "Stack error index out of bounds";
            setCurrentBlockState(-3);
            return;
        }
        path = new Path(path_stack.at(line-1));
    }

    //empty line skip
    if(!text.isEmpty()){
        RuleSet parts = syntax;
        Rule current;
        if(path->length()==0){
            //path empty       => not in body
        }else{
            //path not empty   => in body
            int i = 0;
            while(path->length() > i){
                current = parts.at(path->at(i));
                parts   = current.parts;
                i++;
            }
        }

        int offset = 0; ///@TODO multiple rules per line
        int result = match(text,offset,parts,current,path);

        if(result == -1){
            if(path->empty()){
                //outside of main class
            }else{
                //wrong input
                setCurrentBlockState(-2);
                setFormat(offset,text.length(),err);
            }
        }
    }
    path_stack.append(*path);
}

void Highlighter::highlightBlock(const QString &text)
{
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
        path_stack.clear();
    default:
        //normal operation
        setCurrentBlockState(previousBlockState()+1);
        int line = currentBlockState();

        while(line < path_stack.length()){
            //line number bigger than actual stack number
            path_stack.pop_back();
        }

        find(text,line);
        break;
    }
}
