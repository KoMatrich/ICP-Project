#pragma once

#include "Syntax.h"
#include "Semantics.h"
#include <QString>

typedef QPair<Rule,QString> Lexem;

class Analyzer
{
    typedef QVector<Lexem> LineStack;
    typedef QVector<LineStack> GlobalStack;

public:
    Analyzer(SyntaxTree *syntax_tree){
        this->syntax_tree = syntax_tree;
    }

    void Next(int line, int &offset, const QString &text, Rule &rule);
    void ClearTo(int lineNumber);
    void ClearAll();
private:
    SyntaxTree *syntax_tree;

    GlobalStack global_stack;
    void inline getRules(Rule &current,RuleSet &parts, LineStack stack);

    Lexem *matchBody(const QString &text, int &offset, RuleSet parts);
    int matchEnd(const QString &text, int &offset, Rule current);

    void reduceStack(LineStack *stack);
};
