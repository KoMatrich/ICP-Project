#pragma once

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

class Syntax{
public:
    Syntax();
    QTextCharFormat err,cursor_color,after_err,no_check;
    RuleSet const getRules();
private:
    RuleSet const genRules();
    RuleSet rules;
};
