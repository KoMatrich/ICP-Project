#include "Syntax.h"
#include "assert.h"
#include <QDebug>

SyntaxTree::SyntaxTree()
{
	err.setBackground(Qt::red);
	after_err.setForeground(Qt::darkGray);

	cursor_color.setBackground(Qt::yellow);
	no_check.setForeground(Qt::lightGray);

    genRules();
}

RuleSet const SyntaxTree::getRules() {
    return syntax;
}

QRegExp inline End(const QString input) {
	QRegExp regex = QRegExp(input);
	if (!regex.isValid()) {
		qDebug("Invalid regex:" + regex.pattern().toLatin1());
		exit(99);
	}
	return regex;
}

QRegExp inline Start(const QString input) {
	QRegExp regex = QRegExp(input);
	if (!regex.isValid()) {
		qDebug("Invalid regex:" + regex.pattern().toLatin1());
		exit(99);
	}
	if (!(regex.indexIn("") == -1)) {
		qDebug("Regex matchings empty rule:" + regex.pattern().toLatin1());
		exit(99);
	}
	return regex;
}

///@TODO https://real-world-plantuml.com/umls/4783596598460416

RuleSet const SyntaxTree::genRules() {

	// uml body
    Rule *uml = new Rule();
	uml->start = Start("^@startuml$");
	uml->type = RuleType::MULTI_LINE;
	uml->end = End("^@enduml$");
	uml->format.setFontWeight(QFont::Bold);
	uml->id = RuleID::R_UML;

    // class keyword
    Rule *class_keyword = new Rule();
    class_keyword->start = Start("class");
    class_keyword->format.setFontWeight(QFont::Bold);
    class_keyword->id = RuleID::R_CLASS;

    // interface keyword
    Rule *interface_keyword = new Rule();
    interface_keyword->start = Start("interface");
    interface_keyword->format.setFontWeight(QFont::Bold);
    interface_keyword->id = RuleID::R_INTERFACE;

    // entity name
    Rule *entity_name = new Rule();
    entity_name->start = Start("[A-Za-z0-9_]+");
    entity_name->format.setForeground(Qt::darkYellow);
    entity_name->format.setFontItalic(true);
    entity_name->id = RuleID::R_ENTITYNAME;
    entity_name->wh = RuleWhitespace::W_OPTIONAL;


    // entity name
    Rule *entity_name2 = new Rule();
    entity_name2->start = entity_name->start;
    entity_name2->format.setForeground(Qt::darkYellow);
    entity_name2->format.setFontItalic(true);
    entity_name2->id = RuleID::R_ENTITYNAME;

	// entity block
	Rule *entity_block = new Rule();
    entity_block->start = Start("\\{");
    entity_block->type = RuleType::MULTI_LINE;
    entity_block->end = End("\\}\\s*$");
    entity_block->format.setFontWeight(QFont::Bold);
    entity_block->id = RuleID::R_ENTITYBLOCK;
    entity_block->wh = RuleWhitespace::W_OPTIONAL;


	// method
	Rule *method = new Rule();
	method->start = Start("_?[A-Za-z][A-Za-z0-9_]*\\(\\)");
	method->format.setFontWeight(QFont::Bold);
	method->format.setForeground(Qt::darkBlue);
	method->id = RuleID::R_METHOD;

	// attribute
	Rule *attribute = new Rule();
    attribute->start = Start("_?[A-Za-z][A-Za-z0-9_]*");
    attribute->format.setFontWeight(QFont::Bold);
    attribute->format.setForeground(Qt::darkGreen);
    attribute->id = RuleID::R_ATTRIBUTE;

	// attribute type
	Rule *type = new Rule();
	type->start = Start("(S|s)tring|(I|i)nt|(B|b)ool|(R|r)eal|(F|f)loat|(UI|ui)nt|void");
	type->format.setFontWeight(QFont::Bold);
	type->id = RuleID::R_TYPE;

	// var/method access
	Rule *access = new Rule();
	access->start = Start("[+\\-#~]");
	access->format.setFontWeight(QFont::Bold);
	access->id = RuleID::R_ACCESS;
    access->wh = RuleWhitespace::W_OPTIONAL;

    // in keyword
    Rule *in_kw = new Rule();
    in_kw->start = Start("in");
    in_kw->format.setFontWeight(QFont::Bold);
    in_kw->id = RuleID::R_IN;

    // with keyword
    Rule *with = new Rule();
    with->start = Start("with");
    with->format.setFontWeight(QFont::Bold);
    with->id = RuleID::R_IN;

    // association aggregation composition generalization
    Rule *association = new Rule();
    association->start = Start("ass?o?c?i?a?t?i?o?n?");
    association->format.setFontItalic(true);
    association->id = RuleID::R_ASS;

    Rule *aggregation = new Rule();
    aggregation->start = Start("agg?r?e?g?a?t?i?o?n?");
    aggregation->format.setFontItalic(true);
    aggregation->id = RuleID::R_AGG;

    Rule *composition = new Rule();
    composition->start = Start("co?m?p?o?s?i?t?i?o?n?");
    composition->format.setFontItalic(true);
    composition->id = RuleID::R_COM;

    Rule* generalization = new Rule();
    generalization->start = Start("ge?n?e?r?a?l?i?z?a?t?i?o?n?");
    generalization->format.setFontItalic(true);
    generalization->id = RuleID::R_GEN;

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

    entity_block->parts.push_back(in_kw);
    entity_block->parts.push_back(access);

    entity_name->parts.push_back(entity_block);

    class_keyword->parts.push_back(entity_name);
    interface_keyword->parts.push_back(entity_name);

	uml->parts.push_back(class_keyword);
    uml->parts.push_back(interface_keyword);
	syntax.push_back(uml);

	return syntax;
}
