#pragma once
#include <QString>
#include <QPair>
#include "Syntax.h"
#include "Analyzer.h"
#include "Types.h"

class UMLRelation
{
public:
    UMLRelation() {}
    UMLRelation(QString entity, RuleID type) {
        this->entity = entity;
        this->type = type;
    }
    ~UMLRelation() {}
    QString toString();
    QString getEntity();
    bool updateRelationParams(UMLRelation new_r);
    bool updateRelationConnectors(size_t new_id);
protected:
    QString entity;
    RuleID type;
    size_t id = -1;
};

class UMLProperty
{
public:
    UMLProperty() {}
    UMLProperty(QString mod, QString type, QString name)
    {
        this->p_mod = mod;
        this->p_type = type;
        this->p_name = name;
    }
    ~UMLProperty() {}
    bool operator==(const UMLProperty& rhs)
    {
        return this->p_name == rhs.p_name;
    }
    QString toString();
    bool updateProperty(UMLProperty new_p);
protected:
    QString p_mod;
    QString p_type;
    QString p_name;
};

class UMLClass
{
public:
    UMLClass()
    {
        has_changed = true;
    }
    ~UMLClass() {}
    QString getClassName();
    void updateName(QString name);
    bool has_changed;
    bool has_error = false;
    std::vector<UMLProperty> getAttributes();
    std::vector<UMLProperty> getMethods();
    std::vector<UMLRelation> getRelations();
    void addProperty(UMLProperty new_p, bool isMethod, size_t n);
    void addRelation(UMLRelation new_r, size_t n);
    void removeExceedingProperties(size_t a, size_t m);
    void removeExceedingRelations(size_t r);
    void printProperties();
    bool operator==(const UMLClass& rhs)
    {
        return this->class_name == rhs.class_name;
    }
protected:
    QString class_name = "";
    std::vector<UMLProperty> attributes;
    std::vector<UMLProperty> methods;
    std::vector<UMLRelation> relations;
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
    std::vector<UMLClass> getClasses() { return classes; }
protected:
    std::vector<UMLClass> classes;
    GlobalStack stack;
    void addClass(UMLClass new_class);
    bool skipTreeUntilLastIs(std::vector<RuleID> rules, size_t* index, size_t pos);
    bool skipTreeUntilWhileTrue(std::vector<RuleID> rules, size_t* index, size_t pos, RuleID true_id, size_t true_pos);
    QString getUMLClassName(QString lex);
    void testDuplicates();
    void testRelations();
    void printStack();
private:
    Semantics()
    {
        //instance = this;
    }
    ~Semantics() {}
};
