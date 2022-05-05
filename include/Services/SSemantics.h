#pragma once

#include "Semantics.h"
#include "Services/Code.h"

/// @brief Main semantic checking service
class Semantics : public Service<Semantics>
{
public:
    /// @brief          Builds semantic tree
    /// @param stack    GlobalStack of all Lexems and Rules
    void buildSTree(GlobalStack stack);
    /// @brief          Class getter
    /// @return         Classes checked
    std::vector<UMLClass> getClasses() { return classes; }
    /// @brief          Sequences getter
    /// @return         Sequences checked
    std::vector<Sequence> getSequences() { return sequences; }
    /// @brief          Clears all classes from memory
    void removeClasses();
    /// @brief          Clears all sequences from memory
    void removeSequences();
protected:
    /// @brief          All classes found by semantic
    std::vector<UMLClass> classes;
    /// @brief          All sequences found by semantic
    std::vector<Sequence> sequences;
    /// @brief          GlobalStack copy
    GlobalStack stack;
    /// @brief              Adds new class to memory
    /// @param new_class    UMLClass created by semantic
    void addClass(UMLClass new_class);
    // @TODO @xkrizv03
    bool skipTreeUntilLastIs(std::vector<RuleID> rules, size_t* index, size_t pos);
    bool skipTreeUntilWhileTrue(std::vector<RuleID> rules, size_t* index, size_t pos, RuleID true_id, size_t true_pos);
    void testDuplicates();
    void testRelations();
    void addInheritedProperties();
    void printStack();
    void testProperties();
};
