#include "Semantics.h"
#include "DebugService.h"

QString UMLClass::getClassName()
{
	return this->class_name;
}

void UMLClass::updateName(QString name)
{
	if (this->class_name == name)
		return;

	this->has_changed = true;
	this->class_name = name;
}

bool UMLClass::addAttribute(UMLAttribute new_att)
{
	if (std::find(this->attributes.begin(), this->attributes.end(), new_att) == this->attributes.end()) {
		// new_att not in attributes, add it
		this->attributes.push_back(new_att);
		return true;
	}

	return false;
}

void Semantics::buildSTree(GlobalStack stack)
{
	this->stack = stack;
	VitaClear();
	for (int i = 0; i < this->stack.size(); ++i) {
		VitaPrintf("%1 : %2", VF(i)VF(this->stack[i].size()));
		QString out = "[\"";
		for (int j = 0; j < this->stack[i].size(); j++)
		{
			out.append(toString(this->stack[i][j].first.id));
			out.append("\", \"");
		}
		out.append("END\"]");
		VitaPrint(out);
	}
	size_t i = 0;
	size_t n = 0;
	this->skipTreeUntil(RuleID::R_UML, &i, 0);
	while (this->skipTreeUntil(RuleID::R_CLASS, &i, 1))
	{
		QString c_name = getUMLClassName(this->stack[i][1].second);
		auto a = c_name.toStdString();
		// update class
		if (n < this->classes.size())
		{
			classes[n].updateName(c_name);
		}
		// create class
		else
		{
			UMLClass c = UMLClass();
			c.updateName(c_name);
			this->addClass(c);
		}
		n++;
		i++;
	}
	// delete excesive classes
	classes.resize(n);
}

bool Semantics::addClass(UMLClass new_class)
{
	if (std::find(this->classes.begin(), this->classes.end(), new_class) == this->classes.end()) {
		// new_class not in classes, add it
		this->classes.push_back(new_class);
		return true;
	}

	return false;
}

bool Semantics::eatLexem(Lexem lex)
{
	return true;
}

QString Semantics::getUMLClassName(QString lex)
{
	lex.remove(QRegExp("^class\\s+"));
	lex.remove(QRegExp("\\{$"));
	return lex;
}

bool Semantics::skipTreeUntil(RuleID r_id, size_t* index, size_t pos)
{
	while (*index < this->stack.size())
	{
		if (pos + 1 == this->stack[*index].size())
		{
			if (stack[*index][pos].first.id == r_id) return true;
		}
		(*index)++;
	}
	return false;
}
