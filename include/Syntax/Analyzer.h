#pragma once

#include "Syntax/Rules.h"
#include <QString>
#include "TextDebug.h"
#include "Types.h"

/// @brief Syntax Analyzer
/// that uses SyntaxTree
class Analyzer
{
public:
    /// @brief              Initializes Analyzer
    /// @param syntax_tree  Tree of rules
    Analyzer(SyntaxTree* syntax_tree)
    {
        this->syntax_tree = syntax_tree;
    }
    /// @brief          Finds match at given offset
    /// @param line     line index
    /// @param offset   text offset
    /// @param text     line of text from which is match
    /// @param rule     matched rule
    void Next(int line, int& offset, const QString& text, Rule** rule);
    /// @brief              Clears GlobalStack to line index
    /// @param lineNumber   line index
    /// Finds next lexem in text and updates LineStack.
    /// Offset is lengh of match, if lexem is found.
    /// Offset is unchenged on empty rule match
    /// (used as last rule when matching)
    void ClearTo(int lineNumber);
    /// @brief              Clears GlobalStack
    void ClearAll();
    /// @return             Reference to GlobalStack
    const GlobalStack& GetStack();
private:
    /// @brief pointer to SyntaxTree
    SyntaxTree* syntax_tree;
    /// @brief GlobalStack of rules
    GlobalStack global_stack;
    /// @brief              Gets rule from LineStack
    /// @param current      Current rule (used for end)
    /// @param parts        Current rule parts (to begin new)
    /// @param stack        Current LineStack
    void inline getRules(Rule** current, RuleSet& parts, LineStack stack);
    /// @brief              Matches beginning of one rule from RuleSet
    /// @param text         text from which is match
    /// @param offset       text offset
    /// @param parts        RuleSet for match 
    /// @return             Found match or NULL
    Lexem* matchBody(const QString& text, int& offset, RuleSet parts);
    /// @brief               Matches end of Rule
    /// @param text          text from which is match
    /// @param offset        text offset
    /// @param current       current rule
    /// @return 0  if end is matched
    /// @return 1  if end is empty
    /// @return -1 if end is not found 
    int matchEnd(const QString& text, int& offset, Rule* current);
    /// @brief Pops INLINE rules from LineStack
    void reduceStack(LineStack* stack);
};
