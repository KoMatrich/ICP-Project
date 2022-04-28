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

/// <summary>
/// After class generation, delete exceeding properties
/// </summary>
/// <param name="a">size of attribute list</param>
/// <param name="m">size of method list</param>
void UMLClass::removeExceedingProperties(size_t a, size_t m)
{
    if (this->attributes.size() > a) {
        this->attributes.resize(a);
        this->has_changed = true;
    }

    if (this->methods.size() > m) {
        this->methods.resize(m);
        this->has_changed = true;
    }
}

void UMLClass::printProperties()
{
    for (size_t i = 0; i < attributes.size(); i++)
    {
        VitaPrint(attributes[i].toString());
    }
    for (size_t i = 0; i < methods.size(); i++)
    {
        VitaPrint(methods[i].toString());
    }
}


void Semantics::buildSTree(GlobalStack stack)
{
    this->stack = stack;
    VitaClear();

    size_t i = 0;
    size_t n = 0;
    this->skipTreeUntil(RuleID::R_UML, &i, 0); //TODO fix after new syntax was added
    // find start of a class
    while (this->skipTreeUntilWhileTrue(RuleID::R_CLASS, &i, 1, RuleID::R_UML, 0))
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

        size_t a = 0; // always set to new index
        size_t m = 0;

        // get all attributes / methods
        while (this->skipTreeUntilWhileTrue(RuleID::R_ACCESS, &i, 2, RuleID::R_CLASS, 1)) //w�hilst...  TODO
        {
            if (this->stack[i].size() == 5) // not full
            {
                UMLProperty p = UMLProperty(this->stack[i][2].second, this->stack[i][3].second, this->stack[i][4].second);

                if (this->stack[i][4].first.id == RuleID::R_METHOD)
                    this->classes[n].addProperty(p, true, m++);
                else
                    this->classes[n].addProperty(p, false, a++);
            }
            else
            {
                VitaPrint("[WARNING]: Incomplete property definition");
            }

            if (++i >= this->stack.size()) break;
        }

        this->classes[n].removeExceedingProperties(a, m);

        n++;
    }
    // delete excessive classes
    classes.resize(n);

    for (size_t i = 0; i < classes.size(); i++)
    {
        VitaPrint(classes[i].getClassName());
    }

    testDuplicates();
}

void Semantics::addClass(UMLClass new_class)
{
    this->classes.push_back(new_class);
}

QString Semantics::getUMLClassName(QString lex)
{
    lex.remove(QRegExp("^(class|interface)\\s+"));
    lex.remove(QRegExp("\\{$"));
    return lex;
}

void Semantics::testDuplicates()
{
    if (classes.size() < 2) return; //no duplicates possible

    for (size_t i = 0; i < classes.size() - 1; i++)
    {
        for (size_t j = i + 1; j < classes.size(); j++)
        {
            if (classes[i] == classes[j]) {
                classes[i].has_error = true;
                classes[j].has_error = true;
                VitaPrint("[Semantic error] Duplicate entity name: " + classes[i].getClassName());
            }
        }
    }
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

bool Semantics::skipTreeUntilWhileTrue(RuleID r_id, size_t* index, size_t pos, RuleID true_id, size_t true_pos)
{
    while (*index < this->stack.size())
    {
        if (this->stack[*index].size() <= true_pos) return false;
        if (this->stack[*index][true_pos].first.id != true_id) return false;

        if (pos + 1 == this->stack[*index].size())
        {
            if (stack[*index][pos].first.id == r_id) return true;
        }
        (*index)++;
    }
    return false;
}


QString UMLProperty::toString()
{
    return QString("\t" + p_mod + " " + p_type + " " + p_name);
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
