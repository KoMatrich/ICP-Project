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

	// class
	Rule clas;
	clas.start = Start("class\\s+[A-Za-z0-9_]+\\{$");
	clas.type = RuleType::MULTI_LINE;
	clas.end = End("\\}");
	clas.format.setFontWeight(QFont::Bold);
	clas.id = RuleID::R_CLASS;

	// separators
	Rule separ;
	separ.start = Start("(\\-|=|_){2}");
	separ.format.setFontUnderline(true);
	separ.id = RuleID::R_SEPAR;
	clas.parts.append(separ);

	//method
	Rule method;
	method.start = Start("_?[A-Za-z][A-Za-z0-9_]*\\(\\)");
	method.format.setFontWeight(QFont::Bold);
	method.format.setForeground(Qt::darkBlue);
	method.id = RuleID::R_METHOD;

	// var
	Rule var;
	var.start = Start("_?[A-Za-z][A-Za-z0-9_]*");
	var.format.setFontWeight(QFont::Bold);
	var.format.setForeground(Qt::darkGreen);
	var.id = RuleID::R_VAR;

	// var type
	Rule type;
	type.start = Start("String|Int|Bool|Real|Float|UInt|void");
	type.format.setFontWeight(QFont::Bold);
	//type.id = RuleID::R_TYPE;
	type.parts.append(method);
	type.parts.append(var);

	// var/method access
	Rule access;
	access.start = Start("[+\\-#~]");
	access.format.setFontWeight(QFont::Bold);
	access.parts.append(type);
	access.id = RuleID::R_ACCESS;

	clas.parts.append(access);

	uml.parts.append(clas);

	syntax.append(uml);
	return syntax;
}
