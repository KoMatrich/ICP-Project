#include "Syntax.h"
#include "assert.h"
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
    assert(regex.isValid());
    return regex;
}

QRegExp inline Start(const QString input){
    QRegExp regex = QRegExp(input);
    assert(regex.isValid());
    assert(regex.indexIn("") == -1);
    return regex;
}

RuleSet const Syntax::genRules(){
    RuleSet syntax;

    Rule name;
    name.start = Start("[A-Za-z0-9_]+");
    name.format.setForeground(Qt::darkGreen);

    Rule text_content;
    text_content.start = Start("[A-Za-z0-9_ ]+");
    text_content.format.setForeground(Qt::darkGreen);

    Rule string;
    string.start = Start("\"");
    string.type  = MULTI_LINE;
    string.end   = End("\"");
    string.format.setForeground(Qt::green);
    string.parts.append(text_content);

    //uml body
    Rule uml;
    uml.start = Start("^@startuml");
    uml.type  = MULTI_LINE;
    uml.end   = End("^@enduml");
    uml.format.setFontWeight(QFont::Bold);

    //class
    Rule clas;
    clas.start = Start("^class\\s+[A-Za-z0-9_]+");
    clas.type  = MULTI_LINE;
    clas.format.setFontWeight(QFont::Bold);

    //class atr
    Rule class_atr;
    class_atr.start = Start("^[+#\\-]\\s+");
    class_atr.format.setFontWeight(QFont::Bold);
    class_atr.parts.append(name);
    class_atr.parts.append(string);
    clas.parts.append(class_atr);

    uml.parts.append(clas);
///@todo
//https://java-programming.mooc.fi/part-11/1-class-diagrams
    syntax.append(uml);
    return syntax;
}
