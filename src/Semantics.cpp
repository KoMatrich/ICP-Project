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

void UMLClass::addProperty(UMLProperty new_p, bool isMethod, size_t n)
{
	std::vector<UMLProperty>* search;
	if (isMethod)
		search = &this->methods;
	else
		search = &this->attributes;

	//updating
	if (n < (*search).size())
	{
		this->has_changed |= (*search)[n].updateProperty(new_p);
	}
	//adding
	else
	{
		search->push_back(new_p);
		this->has_changed = true;
	}
}

void Semantics::buildSTree(GlobalStack stack)
{
	this->stack = stack;
	VitaClear();
	for (int i = 0; i < this->stack.size(); ++i) {
		//VitaPrintf("%1 : %2", VF(i)VF(this->stack[i].size()));
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
	// find start of a class
	while (this->skipTreeUntil(RuleID::R_CLASS, &i, 1))
	{
		QString c_name = getUMLClassName(this->stack[i][1].second);

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

		if (++i >= this->stack.size()) break;

		size_t a = 0;
		size_t m = 0;
		// get all attributes / methods
		while (this->stack[i].size() == 5)
		{
			UMLProperty p = UMLProperty(this->stack[i][2].second, this->stack[i][3].second, this->stack[i][4].second);
			

			if (this->stack[i][4].first.id == RuleID::R_METHOD)
				this->classes[n].addProperty(p, true, m++);
			else
				this->classes[n].addProperty(p, false, a++);


			if (++i >= this->stack.size()) break;
		}
		n++;
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
	lex.remove(QRegExp("^(class|interface)\\s+"));
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

bool UMLProperty::updateProperty(UMLProperty new_p)
{
	bool changed = false;
	changed |= new_p.p_mod == this->p_mod;
	changed |= new_p.p_type == this->p_type;
	changed |= new_p.p_name == this->p_name;

	if (changed) {
		this->p_mod = new_p.p_mod;
		this->p_type = new_p.p_type;
		this->p_name = new_p.p_name;
	}

	return changed;
}
