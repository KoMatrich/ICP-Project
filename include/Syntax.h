#pragma once

#include <QTextCharFormat>
#include <QTextEdit>

enum class RuleType {
	INLINE, MULTI_LINE
};

enum class RuleWhitespace {
    W_REQUIRED, W_OPTIONAL
};

enum class RuleID {
	R_ERR = -1, R_UML, R_CLASS, R_INTERFACE, R_ENTITYBLOCK, R_ENTITYNAME, R_METHOD, R_ATTRIBUTE, R_TYPE, R_ACCESS, R_IN, R_ASS, R_AGG, R_COM, R_GEN
};

inline const char* toString(RuleID r)
{
	switch (r)
	{
    
	default:      return "[Unknown]";
	}
}

struct Rule
{
	QRegExp start;
	QRegExp end;
	QTextCharFormat format;
	RuleType type = RuleType::INLINE;
	RuleID id = RuleID::R_ERR;
    RuleWhitespace wh = RuleWhitespace::W_REQUIRED;
	QVector<Rule> parts;
};
typedef QVector<Rule> RuleSet;

class SyntaxTree {
public:
	SyntaxTree();
	QTextCharFormat err, cursor_color, after_err, no_check;

	RuleSet const getRules();
private:
	RuleSet const genRules();
	RuleSet rules;
};

enum SYNTAX_STATE {
	NO_CHECK = -1,
	SYNTAX_ERR = -2
};
enum LINE_STATE {
	FILE_START = -1,
	SYNTAX_E = -2,
	INTERNAL_E = -3
};

