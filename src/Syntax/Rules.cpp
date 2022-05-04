#include "Syntax/Rules.h"

SyntaxTree::SyntaxTree()
{
    err.setBackground(Qt::red);
    after_err.setForeground(Qt::darkGray);

    cursor_color.setBackground(Qt::yellow);
    no_check.setForeground(Qt::lightGray);

    genRules();
}

RuleSet const SyntaxTree::getRules()
{
    return syntax;
}

QRegExp inline End(const QString input)
{
    QRegExp regex = QRegExp(input);
    if (!regex.isValid()) {
        //qDebug("Invalid regex:" + regex.pattern().toLatin1());
        exit(99);
    }
    return regex;
}

QRegExp inline Start(const QString input)
{
    QRegExp regex = QRegExp(input);
    if (!regex.isValid()) {
        //qDebug("Invalid regex:" + regex.pattern().toLatin1());
        exit(99);
    }
    if (!(regex.indexIn("") == -1)) {
        //qDebug("Regex matchings empty rule:" + regex.pattern().toLatin1());
        exit(99);
    }
    return regex;
}

RuleSet const SyntaxTree::genRules()
{
    // uml body
    Rule* uml = new Rule();
    uml->start = Start("^@startuml$");
    uml->type = RuleType::MULTI_LINE;
    uml->end = End("^@enduml$");
    uml->format.setFontWeight(QFont::Bold);
    uml->id = RuleID::R_UML;

    // class keyword
    Rule* class_keyword = new Rule();
    class_keyword->start = Start("class");
    class_keyword->format.setFontWeight(QFont::Bold);
    class_keyword->id = RuleID::R_CLASS;

    // interface keyword
    Rule* interface_keyword = new Rule();
    interface_keyword->start = Start("interface");
    interface_keyword->format.setFontWeight(QFont::Bold);
    interface_keyword->id = RuleID::R_INTERFACE;

    // entity name
    Rule* entity_name = new Rule();
    entity_name->start = Start("[A-Za-z0-9_]+");
    entity_name->format.setForeground(Qt::darkYellow);
    entity_name->format.setFontItalic(true);
    entity_name->id = RuleID::R_ENTITYNAME;
    entity_name->wh = RuleWhitespace::W_OPTIONAL;


    // entity name
    Rule* entity_name2 = new Rule();
    entity_name2->start = entity_name->start;
    entity_name2->format.setForeground(Qt::darkYellow);
    entity_name2->format.setFontItalic(true);
    entity_name2->id = RuleID::R_ENTITYNAME;

    // entity block
    Rule* entity_block = new Rule();
    entity_block->start = Start("\\{");
    entity_block->type = RuleType::MULTI_LINE;
    entity_block->end = End("\\}\\s*$");
    entity_block->format.setFontWeight(QFont::Bold);
    entity_block->id = RuleID::R_ENTITYBLOCK;
    entity_block->wh = RuleWhitespace::W_OPTIONAL;


    // method
    Rule* method = new Rule();
    method->start = Start("_?[A-Za-z][A-Za-z0-9_]*\\(\\)$");
    method->format.setFontWeight(QFont::Bold);
    method->format.setForeground(Qt::darkBlue);
    method->id = RuleID::R_METHOD;

    // attribute
    Rule* attribute = new Rule();
    attribute->start = Start("_?[A-Za-z][A-Za-z0-9_]*$");
    attribute->format.setFontWeight(QFont::Bold);
    attribute->format.setForeground(Qt::darkGreen);
    attribute->id = RuleID::R_ATTRIBUTE;

    // attribute type
    Rule* type = new Rule();
    type->start = Start("(S|s)tring|(I|i)nt|(B|b)ool|(R|r)eal|(F|f)loat|(UI|ui)nt|void");
    type->format.setFontWeight(QFont::Bold);
    type->id = RuleID::R_TYPE;

    // var/method access
    Rule* access = new Rule();
    access->start = Start("[+\\-#~]");
    access->format.setFontWeight(QFont::Bold);
    access->id = RuleID::R_ACCESS;
    access->wh = RuleWhitespace::W_OPTIONAL;

    // in keyword
    Rule* in_kw = new Rule();
    in_kw->start = Start("in");
    in_kw->format.setFontWeight(QFont::Bold);
    in_kw->id = RuleID::R_IN;

    // with keyword
    Rule* with = new Rule();
    with->start = Start("with");
    with->format.setFontWeight(QFont::Bold);
    with->id = RuleID::R_IN;

    // association aggregation composition generalization
    Rule* association = new Rule();
    association->start = Start("ass?o?c?i?a?t?i?o?n?");
    association->format.setFontItalic(true);
    association->id = RuleID::R_ASS;

    Rule* aggregation = new Rule();
    aggregation->start = Start("agg?r?e?g?a?t?i?o?n?");
    aggregation->format.setFontItalic(true);
    aggregation->id = RuleID::R_AGG;

    Rule* composition = new Rule();
    composition->start = Start("co?m?p?o?s?i?t?i?o?n?");
    composition->format.setFontItalic(true);
    composition->id = RuleID::R_COM;

    Rule* generalization = new Rule();
    generalization->start = Start("ge?n?e?r?a?l?i?z?a?t?i?o?n?");
    generalization->format.setFontItalic(true);
    generalization->id = RuleID::R_GEN;

    // position
    Rule* position = new Rule();
    position->start = Start("\\-?\\d+$");
    position->format = posValFormat();
    position->id = RuleID::R_POSVAL;

    // x pos
    Rule* x_pos = new Rule();
    x_pos->start = Start("x:");
    x_pos->format = posFormat();
    x_pos->id = RuleID::R_XPOS;

    // y pos
    Rule* y_pos = new Rule();
    y_pos->start = Start("y:");
    y_pos->format = posFormat();
    y_pos->id = RuleID::R_YPOS;

    // parts appending

    with->parts.push_back(entity_name2);

    association->parts.push_back(with);
    aggregation->parts.push_back(with);
    composition->parts.push_back(with);
    generalization->parts.push_back(with);

    type->parts.push_back(method);
    type->parts.push_back(attribute);

    in_kw->parts.push_back(association);
    in_kw->parts.push_back(aggregation);
    in_kw->parts.push_back(composition);
    in_kw->parts.push_back(generalization);
    access->parts.push_back(type);
    x_pos->parts.push_back(position);
    y_pos->parts.push_back(position);

    entity_block->parts.push_back(x_pos);
    entity_block->parts.push_back(y_pos);
    entity_block->parts.push_back(in_kw);
    entity_block->parts.push_back(access);

    entity_name->parts.push_back(entity_block);

    class_keyword->parts.push_back(entity_name);
    interface_keyword->parts.push_back(entity_name);

    uml->parts.push_back(class_keyword);
    uml->parts.push_back(interface_keyword);

    syntax.push_back(uml);

    /* ------------- SEQ ----------------*/

    // sequence diagram
    Rule* seq = new Rule();
    seq->start = Start("^@startseq$");
    seq->type = RuleType::MULTI_LINE;
    seq->end = End("^@endseq$");
    seq->format.setFontWeight(QFont::Bold);
    seq->id = RuleID::R_SEQ;

    // activate keyword
    Rule* activate_keyword = new Rule();
    activate_keyword->start = Start("activate");
    activate_keyword->format.setFontWeight(QFont::Bold);
    activate_keyword->format.setForeground(Qt::darkGreen);
    activate_keyword->id = RuleID::R_ACTIVATE;

    // deactivate keyword
    Rule* deactivate_keyword = new Rule();
    deactivate_keyword->start = Start("deactivate");
    deactivate_keyword->format.setFontWeight(QFont::Bold);
    deactivate_keyword->format.setForeground(Qt::darkRed);
    deactivate_keyword->id = RuleID::R_DEACTIVATE;

    // entity name
    Rule* x_tivate = new Rule();
    x_tivate->start = Start("[A-Za-z0-9_]+$");
    x_tivate->format.setForeground(Qt::darkYellow);
    x_tivate->format.setFontItalic(true);
    x_tivate->id = RuleID::R_ENTITYNAME;

    // sender entity
    Rule* sender = new Rule();
    sender->start = Start("[A-Za-z0-9_]+");
    sender->format.setForeground(Qt::darkYellow);
    sender->format.setFontItalic(true);
    sender->id = RuleID::R_ENTITYNAME;

    // arrow sync
    Rule* a_sync = new Rule();
    a_sync->start = Start("\\-\\>");
    a_sync->format.setForeground(Qt::darkMagenta);
    a_sync->id = RuleID::R_ARROW_SYNC;

    // arrow sync
    Rule* a_async = new Rule();
    a_async->start = Start("\\-\\-\\>");
    a_async->format.setForeground(Qt::darkMagenta);
    a_async->id = RuleID::R_ARROW_ASYNC;

    // receiver entity
    Rule* receiver = new Rule();
    receiver->start = Start("[A-Za-z0-9_]+");
    receiver->format.setForeground(Qt::darkYellow);
    receiver->format.setFontItalic(true);
    receiver->id = RuleID::R_ENTITYNAME;
    receiver->wh = RuleWhitespace::W_OPTIONAL;

    // colon
    Rule* colon = new Rule();
    colon->start = Start("\\:");
    colon->id = RuleID::R_COLON;

    // message
    Rule* message = new Rule();
    message->start = Start("_?[A-Za-z][A-Za-z0-9_]*\\(\\)$");
    message->format.setFontWeight(QFont::Bold);
    message->format.setForeground(Qt::darkBlue);
    message->id = RuleID::R_METHOD;

    colon->parts.push_back(message);

    receiver->parts.push_back(colon);

    a_async->parts.push_back(receiver);
    a_sync->parts.push_back(receiver);

    sender->parts.push_back(a_async);
    sender->parts.push_back(a_sync);
    activate_keyword->parts.push_back(x_tivate);
    deactivate_keyword->parts.push_back(x_tivate);


    seq->parts.push_back(deactivate_keyword);
    seq->parts.push_back(activate_keyword);
    seq->parts.push_back(sender);

    syntax.push_back(seq);

    return syntax;
}
