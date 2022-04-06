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

QRegExp inline Regex(const QString input){
    QRegExp regex = QRegExp(input);
    if(!regex.isValid()){
        qDebug() << "Invalid syntax \"" << input << "\"" ;
        exit(1);
    }
    return regex;
}

RuleSet const Syntax::genRules(){
    RuleSet syntax;

    //uml body
    Rule uml;
    uml.start = Regex("^@startuml$");
    uml.end   = Regex("^@enduml$");
    uml.type  = MULTI_LINE;
    uml.format.setFontWeight(QFont::Bold);

    //title
    Rule title;
    title.start = Regex("^title ");
    title.end   = Regex("$");
    title.format.setFontWeight(QFont::Bold);
    uml.parts.append(title);

    syntax.append(uml);
    return syntax;
}
