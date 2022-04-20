#include "Syntax.h"
#include "assert.h"
#include <QDebug>

SyntaxTree::SyntaxTree()
{
    err.setBackground(Qt::red);
    after_err.setForeground(Qt::darkGray);

    cursor_color.setBackground(Qt::yellow);
    no_check.setForeground(Qt::lightGray);

    rules = genRules();
}

RuleSet const SyntaxTree::getRules(){
    return rules;
}

QRegExp inline End(const QString input){
    QRegExp regex = QRegExp(input);
    if(!regex.isValid()){
        qDebug("Invalid regex:"+regex.pattern().toLatin1());
        exit(99);
    }
    return regex;
}

QRegExp inline Start(const QString input){
    QRegExp regex = QRegExp(input);
    if(!regex.isValid()){
        qDebug("Invalid regex:"+regex.pattern().toLatin1());
        exit(99);
    }
    if(!regex.indexIn("") == -1){
        qDebug("Regex matchings empty rule:"+regex.pattern().toLatin1());
        exit(99);
    }
    return regex;
}

///@TODO https://real-world-plantuml.com/umls/4783596598460416

RuleSet const SyntaxTree::genRules(){
    RuleSet syntax;

    //uml body
    Rule uml;
    uml.start   = Start("^@startuml$");
    uml.type    = MULTI_LINE;
    uml.end     = End("^@enduml$");
    uml.format.setFontWeight(QFont::Bold);

    //class
    Rule clas;
    clas.start  = Start("class\\s+[A-Za-z0-9_]+\\{$");
    clas.type   = MULTI_LINE;
    clas.end    = End("\\}");
    clas.format.setFontWeight(QFont::Bold);

    //separators
    Rule separ;
    separ.start  = Start("(\\-|\\.|_){2}");
    separ.format.setFontUnderline(true);
    clas.parts.append(separ);

    //method
    Rule method;
    method.start = Start("_?[A-Za-z][A-Za-z0-9_]*\\(\\)");
    method.format.setFontWeight(QFont::Bold);
    method.format.setForeground(Qt::darkBlue);
    clas.parts.append(method);

    //var
    Rule var;
    var.start = Start("_?[A-Za-z][A-Za-z0-9_]*");
    var.format.setFontWeight(QFont::Bold);
    var.format.setForeground(Qt::darkGreen);
    clas.parts.append(var);

    //class access
    Rule type;
    type.start = Start("[+\\-#~]");
    type.format.setFontWeight(QFont::Bold);
    type.parts.append(method);
    type.parts.append(var);
    clas.parts.append(type);

    uml.parts.append(clas);

    syntax.append(uml);
    return syntax;
}
