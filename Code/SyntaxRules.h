#ifndef SYNTAX_H
#define SYNTAX_H

#include <QTextCharFormat>
#include <QTextEdit>

enum RuleType{
    INLINE, NORMAL,
};

struct Rule
{
    QRegExp start;
    QRegExp end;
    QTextCharFormat format;
    RuleType rule_type;

    QVector<Rule> parts;
};

typedef QVector<Rule> RuleSet;

RuleSet const Syntax();
RuleSet const syntax = Syntax();
#endif // SYNTAX_H
