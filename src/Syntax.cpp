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

RuleSet const SyntaxTree::getRules() {
	return rules;
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
	RuleSet syntax;

	// uml body
	Rule uml;
	uml.start = Start("^@startuml$");
	uml.type = RuleType::MULTI_LINE;
	uml.end = End("^@enduml$");
	uml.format.setFontWeight(QFont::Bold);
	uml.id = RuleID::R_UML;

    // class keyword
    Rule class_keyword;
    class_keyword.start = Start("class");
    class_keyword.format.setFontWeight(QFont::Bold);
    class_keyword.id = RuleID::R_ENTITYNAME;

    // interface keyword
    Rule interface_keyword;
    interface_keyword.start = Start("interface");
    interface_keyword.format.setFontWeight(QFont::Bold);
    interface_keyword.id = RuleID::R_ENTITYNAME;

    // entity name
    Rule entity_name;
    entity_name.start = Start("[A-Za-z0-9_]+");
    entity_name.format.setForeground(Qt::darkYellow);
    entity_name.format.setFontItalic(true);
    entity_name.id = RuleID::R_ENTITYNAME;
    entity_name.wh = RuleWhitespace::W_OPTIONAL;


    // entity name
    Rule entity_name2;
    entity_name2.start = entity_name.start;
    entity_name2.format.setForeground(Qt::darkYellow);
    entity_name2.format.setFontItalic(true);
    entity_name2.id = RuleID::R_ENTITYNAME;

	// entity block
	Rule entity_block;
    entity_block.start = Start("\\{");
    entity_block.type = RuleType::MULTI_LINE;
    entity_block.end = End("\\}\\s*$");
    entity_block.format.setFontWeight(QFont::Bold);
    entity_block.id = RuleID::R_ENTITYBLOCK;
    entity_block.wh = RuleWhitespace::W_OPTIONAL;


	// method
	Rule method;
	method.start = Start("_?[A-Za-z][A-Za-z0-9_]*\\(\\)");
	method.format.setFontWeight(QFont::Bold);
	method.format.setForeground(Qt::darkBlue);
	method.id = RuleID::R_METHOD;

	// attribute
	Rule attribute;
    attribute.start = Start("_?[A-Za-z][A-Za-z0-9_]*");
    attribute.format.setFontWeight(QFont::Bold);
    attribute.format.setForeground(Qt::darkGreen);
    attribute.id = RuleID::R_ATTRIBUTE;

	// attribute type
	Rule type;
	type.start = Start("(S|s)tring|(I|i)nt|(B|b)ool|(R|r)eal|(F|f)loat|(UI|ui)nt|void");
	type.format.setFontWeight(QFont::Bold);
	type.id = RuleID::R_TYPE;

	// var/method access
	Rule access;
	access.start = Start("[+\\-#~]");
	access.format.setFontWeight(QFont::Bold);
	access.id = RuleID::R_ACCESS;
    access.wh = RuleWhitespace::W_OPTIONAL;

    // in keyword
    Rule in_kw;
    in_kw.start = Start("in");
    in_kw.format.setFontWeight(QFont::Bold);
    in_kw.id = RuleID::R_IN;

    // with keyword
    Rule with;
    with.start = Start("with");
    with.format.setFontWeight(QFont::Bold);
    with.id = RuleID::R_IN;

    // association aggregation composition generalization
    Rule association;
    association.start = Start("ass?o?c?i?a?t?i?o?n?");
    association.format.setFontItalic(true);
    association.id = RuleID::R_ASS;

    Rule aggregation;
    aggregation.start = Start("agg?r?e?g?a?t?i?o?n?");
    aggregation.format.setFontItalic(true);
    aggregation.id = RuleID::R_AGG;

    Rule composition;
    composition.start = Start("co?m?p?o?s?i?t?i?o?n?");
    composition.format.setFontItalic(true);
    composition.id = RuleID::R_COM;

    Rule generalization;
    generalization.start = Start("ge?n?e?r?a?l?i?z?a?t?i?o?n?");
    generalization.format.setFontItalic(true);
    generalization.id = RuleID::R_GEN;

    // parts appending

    with.parts.append(&entity_name2);

    association.parts.append(&with);
    aggregation.parts.append(&with);
    composition.parts.append(&with);
    generalization.parts.append(&with);

    type.parts.append(&method);
    type.parts.append(&attribute);

    in_kw.parts.append(&association);
    in_kw.parts.append(&aggregation);
    in_kw.parts.append(&composition);
    in_kw.parts.append(&generalization);
    access.parts.append(&type);

    entity_block.parts.append(&in_kw);
    entity_block.parts.append(&access);

    entity_name.parts.append(&entity_block);

    class_keyword.parts.append(&entity_name);
    interface_keyword.parts.append(&entity_name);

	uml.parts.append(&class_keyword);
    uml.parts.append(&interface_keyword);
	syntax.append(&uml);

	return syntax;
}
