#pragma once

#include <QTextCharFormat>
#include <QTextEdit>

enum RuleType{
    INLINE,MULTI_LINE
};

struct Rule
{
    QRegExp start;
    QRegExp end;
    QTextCharFormat format;
    RuleType type = INLINE;

    QVector<Rule> parts;
};
typedef QVector<Rule> RuleSet;

class SyntaxTree{
public:
    SyntaxTree();
    QTextCharFormat err,cursor_color,after_err,no_check;

    RuleSet const getRules();
private:
    RuleSet const genRules();
    RuleSet rules;
};

enum SYNTAX_STATE{
    NO_CHECK    =-1,
    SYNTAX_ERR  =-2
};
enum LINE_STATE{
    FILE_START  =-1,
    SYNTAX_E    =-2,
    INTERNAL_E  =-3
};

