#pragma once

#include "Graph/ERD/View.h"
#include "Graph/SEQ/View.h"

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
    /// @brief          Sets endpoint for refreshing
    /// @param endpoint endpoint
    void setERDView(ERDView* endpoint);
    /// @brief          Sets endpoint for refreshing
    /// @param endpoint endpoint
    void setSEQView(SEQView* endpoint);
protected:
    /// @brief          endpoint for ERDView
    ERDView* erd = nullptr;
    /// @brief          endpoint for SEQView
    SEQView* seq = nullptr;

    /// @brief          All classes found by semantic
    std::vector<UMLClass> classes;
    /// @brief          All sequences found by semantic
    std::vector<Sequence> sequences;
    /// @brief          GlobalStack copy
    GlobalStack stack;
    /// @brief              Adds new class to memory
    /// @param new_class    UMLClass created by semantic

    void addClass(UMLClass new_class);
    /// @brief          Reads lines of global stack until the last element at pos is of type rule
    /// @param rules    vector of acceptable rules
    /// @param index    index pointer (will be incremented outside of the function)
    /// @param pos      position in a linestack
    /// @return         returns true on find and false if skipped to the end of global stack
    bool skipTreeUntilLastIs(std::vector<RuleID> rules, size_t* index, size_t pos);
    /// @brief          Reads lines of global stack until linestack element at position pos is
    ///                 in rules vector or the condition (true_id at true_pos) is invalid
    /// @param rules    vector of acceptable rules
    /// @param index    index pointer (will be incremented outside of the function)
    /// @param pos      position in a linestack
    /// @param true_id  condition - id of required rule
    /// @param true_pos condition - position of required rule
    /// @return         returns true on find and false if skipped to the end of global stack
    bool skipTreeUntilWhileTrue(std::vector<RuleID> rules, size_t* index, size_t pos, RuleID true_id, size_t true_pos);
    /// @brief          tests classes for duplicate names and sets error flags
    void testDuplicates();
    /// @brief          tests class relations and connects them by class id
    void testRelations();
    /// @brief          in case of generalization, copies all inherited methods and attributes to specialized child
    void addInheritedProperties();
    /// @brief          Debug - prints the entire global stack into Debug window
    void printStack();
    /// @brief          Tests if names of properties (both own and inherited) are not colliding and sets error flags
    void testProperties();
};
