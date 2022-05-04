#pragma once

#include "Semantics.h"
#include "Services/Code.h"

class Semantics : public Service<Semantics>
{
public:
    void buildSTree(GlobalStack stack);
    std::vector<UMLClass> getClasses() { return classes; }
protected:
    std::vector<UMLClass> classes;
    GlobalStack stack;
    void addClass(UMLClass new_class);
    bool skipTreeUntilLastIs(std::vector<RuleID> rules, size_t* index, size_t pos);
    bool skipTreeUntilWhileTrue(std::vector<RuleID> rules, size_t* index, size_t pos, RuleID true_id, size_t true_pos);
    void testDuplicates();
    void testRelations();
    void addInheritedProperties();
    void printStack();
    void testProperties();
};
