#include "Semantics.h"

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

bool Semantics::addClass(UMLClass new_class)
{
    if (std::find(this->classes.begin(), this->classes.end(), new_class) == this->classes.end()) {
        // new_class not in classes, add it
        this->classes.push_back(new_class);
        return true;
    }

    return false;
}

bool Semantics::eatQPair(QPair<Rule, QString> pair)
{
    return true;
}
