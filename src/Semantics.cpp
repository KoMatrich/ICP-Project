#include "Semantics.h"
#include "DebugService.h"

QString UMLClass::getClassName()
{
	return this->class_name;
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
	VitaClear();
	for (int i = 0; i < stack.size(); ++i) {
		VitaPrintf("%1 : %2", VF(i)VF(stack[i].size()));
		QString out = "[\"";
		for (int j = 0; j < stack[i].size(); j++)
		{
			out.append(toString(stack[i][j].first.id));
			out.append("\", \"");
		}
		out.append("END\"]");
		VitaPrint(out);
	}
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
