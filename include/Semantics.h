#pragma once
#include <QString>
#include <QPair>
#include "Syntax.h"
#include "Analyzer.h"
#include "Types.h"

class UMLAttribute
{
public:
	UMLAttribute() {}
	~UMLAttribute() {}
	bool operator==(const UMLAttribute& rhs)
	{
		return this->att_name == rhs.att_name;
	}
protected:
	QString att_mod;
	QString att_type;
	QString att_name;
};

class UMLClass
{
public:
	UMLClass() {
		has_changed = true;
	}
	~UMLClass() {}
	QString getClassName();
	void updateName(QString name);
	bool has_changed;
	bool addAttribute(UMLAttribute new_att);
	bool operator==(const UMLClass& rhs)
	{
		return this->class_name == rhs.class_name;
	}
protected:
	QString class_name = "";
	std::vector<UMLAttribute> attributes;
};

class Semantics
{
private:
	static Semantics* instance;
public:
	static Semantics* getInstance()
	{
		if (!instance)
			instance = new Semantics;
		return instance;
	}
	void buildSTree(GlobalStack stack);
protected:
	std::vector<UMLClass> classes;
	GlobalStack stack;
	bool addClass(UMLClass new_class);
	bool eatLexem(Lexem lex);
	bool skipTreeUntil(RuleID r_id, size_t* index, size_t pos);
	QString getUMLClassName(QString lex);
private:
	Semantics() {
		//instance = this;
	}
	~Semantics() {}
};
