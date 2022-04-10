#pragma once
#include <QString>
#include <QPair>
#include "Syntax.h"

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
    UMLClass() {}
    ~UMLClass() {}
    QString getClassName();
    bool addAttribute(UMLAttribute new_att);
    bool operator==(const UMLClass& rhs)
    {
        return this->class_name == rhs.class_name;
    }
protected:
    QString class_name;
    std::vector<UMLAttribute> attributes;
};

class Semantics
{
public:
    Semantics() {}
    ~Semantics() {}
    bool addClass(UMLClass new_class);
    bool eatQPair(QPair<Rule, QString> pair);
protected:
    std::vector<UMLClass> classes;
};
