#include "Syntax.h"
#include <QDebug>

QRegExp inline Regex(const QString input){
    QRegExp regex = QRegExp(input);
    if(!regex.isValid()){
        qDebug() << "Invalid syntax \"" << input << "\"" ;
    }
    return regex;
}

RuleSet const Syntax(){
    RuleSet syntax;

    const QString _name = "[_A-Za-z0-9]+";
    const QString _forms = "[ \\{\\}\\[\\]\\(\\)\\!\\_\\@\\#\\$\\%\\^\\&\\+\\-\\*\\/\\=\\?\\.\\,\\'\"\\;\\:\\<\\>\\\\]";
    const QString _sentence = "("+_name+"|"+_forms+"*)+";

    Rule sentence;
    sentence.start = Regex("^"+_sentence+"$");

    Rule sentence_f;
    sentence_f.start = Regex("^([\\*]{2})"+_sentence+"\\1$");

    Rule string;
    string.start = Regex("\""+_sentence);
    string.end = Regex("\"");
    string.format.setForeground(Qt::green);


    //uml body
    Rule uml;
    uml.start = Regex("^@startuml$");
    uml.end   = Regex("^@enduml$");
    uml.format.setFontWeight(QFont::Bold);

    //title
    Rule title;
    title.start = Regex("^title "+_sentence+"$");
    title.format.setFontWeight(QFont::Bold);
    uml.parts.append(title);

    //skin
    Rule skin;
    skin.start = Regex("^skin "+_sentence+"$");
    uml.parts.append(skin);

    //namedef
    Rule namedef;
    namedef.start = Regex("^'?participant \""+_name+"\" as "+_name+"$");
    uml.parts.append(namedef);

    //activate
    Rule activate;
    activate.start = Regex("^activate "+_name+"$");
    uml.parts.append(activate);

    //deactivate
    Rule deactivate;
    deactivate.start = Regex("^deactivate "+_name+"$");
    uml.parts.append(deactivate);

    //transmision
    Rule transmision;
    transmision.start = Regex("^"+_name+" (-->|->) "+_name+": "+_sentence+"$");
    uml.parts.append(transmision);
    //class atribute
    Rule atr;
    atr.start = Regex("^(\\*{0,3})"+_sentence+"\\1$");

    //class separator
    Rule sep;
    sep.start = Regex("^(([.\\-=]{2})"+_sentence+"\\2)|([.\\-=]{4})$");
    sep.format.setUnderlineStyle(QTextCharFormat::SingleUnderline);

    //class
    Rule clas;
    clas.start = Regex("^class "+_name+"$");
    clas.end   = Regex("^end class$");
    clas.format.setFontWeight(QFont::Bold);
    clas.parts.append(atr);
    clas.parts.append(sep);
    uml.parts.append(clas);

    //object
    Rule object;
    object.start = Regex("^object "+_name+"$");
    object.end   = Regex("^end$");
    object.format.setFontWeight(QFont::Bold);
    object.parts.append(atr);
    object.parts.append(sep);
    object.parts.append(sentence_f);
    uml.parts.append(object);

    //block_note
    Rule b_note;
    b_note.start = Regex("^note (left|right)$");
    b_note.end   = Regex("^end note$");
    b_note.format.setFontWeight(QFont::Bold);
    b_note.format.setForeground(Qt::darkGreen);

    Rule note_body;
    note_body.format.setForeground(Qt::green);
    b_note.parts.append(note_body);
    uml.parts.append(b_note);

    //line note
    Rule l_note;
    l_note.start = Regex("^note (left|right): "+_sentence+"$");
    l_note.format.setFontWeight(QFont::Bold);
    l_note.format.setForeground(Qt::darkGreen);
    uml.parts.append(l_note);

    syntax.append(uml);
    return syntax;
}
