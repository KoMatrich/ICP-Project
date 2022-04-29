#pragma once
#include <QString>
#include <QPair>
#include "Syntax.h"
#include "Analyzer.h"
#include "Types.h"
#include "DebugService.h"
#include "HighlightService.h"
#include "CodeService.h"

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
    UMLClass() {}
    ~UMLClass() {}
    QString getClassName();
    void updateName(QString name);
    bool has_changed = true;
    inline std::vector<UMLProperty> getAttributes() { return attributes; }
    inline std::vector<UMLProperty> getMethods() { return methods; }
    inline std::vector<UMLRelation> getRelations() { return relations; }
    inline int getXPos() { return x; }
    inline int getYPos() { return y; }
    void addProperty(UMLProperty new_p, bool isMethod, size_t n);
    void addRelation(UMLRelation new_r, size_t n);
    bool updatePosition(int pos, bool is_x);
    void removeExceedingProperties(size_t a, size_t m);
    void removeExceedingRelations(size_t r);
    void printProperties();
    void setErrorFlag(bool e) { has_changed |= (had_error != e); had_error = has_error; has_error = e; }
    inline bool getErrorFlag() { return has_error; }
    inline void removePosFlags() { x_set = false; y_set = false; }
    inline bool getDuplicateFlag() { return duplicate; }
    inline void setDuplicateFlag(bool d) { duplicate = d; }
    bool operator==(const UMLClass& rhs)
    {
        return this->class_name == rhs.class_name;
    }
protected:
    bool duplicate = false;
    bool has_error = false;
    bool had_error = true;
    QString class_name = "";
    std::vector<UMLProperty> attributes;
    std::vector<UMLProperty> methods;
    std::vector<UMLRelation> relations;
    int x = 0;
    int y = 0;
    bool x_set = false;
    bool y_set = false;
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
