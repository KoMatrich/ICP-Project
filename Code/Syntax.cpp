#include "Syntax.h"
#include <QDebug>

Syntax::Syntax()
{
    err.setBackground(Qt::red);
    after_err.setForeground(Qt::darkGray);

    cursor_color.setBackground(Qt::yellow);
    no_check.setForeground(Qt::lightGray);

    rules = genRules();
}

RuleSet const Syntax::getRules(){
    return rules;
}

QRegExp inline End(const QString input){
    QRegExp regex = QRegExp(input);
    if(!regex.isValid()){
        qDebug() << "Invalid syntax! \"" << input << "\"" ;
        exit(1);
    }
    return regex;
}

QRegExp inline Start(const QString input){
    QRegExp regex = QRegExp(input);
    if(!regex.isValid()){
        qDebug() << "Invalid syntax! \"" << input << "\"" ;
        exit(1);
    }
    if(regex.indexIn("") != -1){
        qDebug() << "Syntax matches empty space! \"" << input << "\"" ;
        exit(1);
    }
    return regex;
}

RuleSet const Syntax::genRules(){
    RuleSet syntax;

    Rule name;
    name.start = Start("[A-Za-z0-9_]+");
    name.format.setForeground(Qt::green);

    //uml body
    Rule uml;
    uml.start = Start("^@startuml");
    uml.end   = End("^@enduml");
    uml.type  = MULTI_LINE;
    uml.format.setFontWeight(QFont::Bold);

    //class
    Rule clas;
    clas.start = Start("^class");
    clas.type  = MULTI_LINE;
    clas.format.setFontWeight(QFont::Bold);



    uml.parts.append(clas);

    syntax.append(uml);
    return syntax;
}
