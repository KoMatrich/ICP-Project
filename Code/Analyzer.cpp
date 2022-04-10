#include "Analyzer.h"
#include "QDebug"

Analyzer::Analyzer(Syntax *syntax)
{
    this->syntax = syntax;
}

void inline Analyzer::getRules(Rule &current,RuleSet &parts, Path path){
    parts = syntax->getRules();
    current = Rule();
    current.type = MULTI_LINE;

    if(path.length()!=0){
        //path not empty   => in body
        for(int i = 0; i < path.length(); i++){
            current = parts.at(path.at(i));
            parts   = current.parts;
        }
    }
}

///If found index of matched rule, else -1
int Analyzer::matchBody(const QString &text, int &offset, RuleSet parts)
{
    //find start of sub block
    for(int i = 0; i < parts.length();i++){
        Rule part = parts.at(i);
        QRegExp start = part.start;
        int match_i = start.indexIn(text,offset);
        if(match_i == offset){

            qDebug("Matched:part%d/%d.start",i,parts.length());
            qDebug("\t"+text.mid(offset,start.matchedLength()).toLatin1());
            qDebug("\t"+part.start.pattern().toLatin1());

            offset += start.matchedLength();
            return i;
        }
    }

    //no rule was matched
    return -1;
}

///If found returns 0, else -1
int Analyzer::matchEnd(const QString &text, int &offset, Rule current)
{
    //find end of current block
    QRegExp end = current.end;
    if(!end.isEmpty()){
        int match_i = end.indexIn(text,offset);
        if(match_i == offset){

            qDebug("Matched:body.end");
            qDebug("\t"+text.mid(offset,end.matchedLength()).toLatin1());
            qDebug("\t"+current.end.pattern().toLatin1());

            offset += end.matchedLength();
            return 0;
        }
    }else{
        //block without ending
        qDebug("Ending :body.end");
        qDebug("\t Empty patern");

        return 1;
    }

    //no rule was matched
    return -1;
}

void Analyzer::reducePath(Path *path)
{
    while(path->length()!=0){
        RuleSet parts = syntax->getRules();
        Rule current;

        for(int i = 0; i < path->length(); i++){
            current = parts.at(path->at(i));
            parts   = current.parts;
        }

        if(current.end.isEmpty()&&current.type!=MULTI_LINE){
            path->removeLast();
        }else{
            break;
        }
    }
}

void Analyzer::Next(int line, int &offset, const QString &text, Rule &current)
{
    //path to current block
    Path *path;
    RuleSet parts;

    if(path_stack.length() == line+1){
        //recursive call
        path = new Path(path_stack.at(line));
    }else{
        //first call of this function on this line
        if(line == 0){
            //first line
            path = new Path();
        }else{
            //load stack from previous line
            path = new Path(path_stack.at(line-1));
            reducePath(path);
        }
    }

    getRules(current,parts,*path);
    int result;

    if((offset==0) && (current.type==INLINE)){
        offset = SYNTAX_E;
        qDebug() << "Previus rule didn't end well";
        goto SKIP;
    }

    result = matchBody(text,offset,parts);
    if(result>=0){
        path->append(result);
        getRules(current,parts,*path);
        goto SKIP;
    }

    if(path->size()==0){
        //not in body so no checking
        offset = NO_CHECK;
        goto SKIP;
    }

    result = matchEnd(text,offset,current);
    if(result>=0){
        path->removeLast();
        goto SKIP;
    }

    //nothing found but text is still remaining
    offset = SYNTAX_E;
SKIP:
    ClearTo(line);
    path_stack.append(*path);
}

void Analyzer::ClearTo(int lineNumber)
{
    while(lineNumber < path_stack.length()){
        //line number bigger than actual stack index
        //needs to remove overhead
        path_stack.removeLast();
    }
}

void Analyzer::ClearAll()
{
    path_stack.clear();
}

