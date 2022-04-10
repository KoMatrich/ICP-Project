#pragma once

#include "Syntax.h"
#include "Semantics.h"
#include <QString>

typedef QVector<int> Path;
typedef QVector<Path> PathStack;

class Analyzer
{
public:
    Analyzer(Syntax *syntax);

    void Next(int line, int &offset, const QString &text, Rule &rule);
    void ClearTo(int lineNumber);
    void ClearAll();
private:
    Syntax *syntax;

    PathStack path_stack;
    void inline getRules(Rule &current,RuleSet &parts, Path path);

    int matchBody(const QString &text, int &offset, RuleSet parts);
    int matchEnd(const QString &text, int &offset, Rule current);

    void reducePath(Path *path);
};
