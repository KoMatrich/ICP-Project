#pragma once

#include <QTextCharFormat>
#include <QTextEdit>

enum class RuleType {
	INLINE, MULTI_LINE
};

enum class RuleID {
	R_ERR = -1, R_UML, R_CLASS, R_SEPAR, R_METHOD, R_VAR, R_ACCESS
};

inline const char* toString(RuleID r)
{
	switch (r)
	{
	case RuleID::R_ERR:    return "R_ERR";
	case RuleID::R_UML:    return "R_UML";
	case RuleID::R_CLASS:  return "R_CLASS";
	case RuleID::R_SEPAR:  return "R_SEPAR";
	case RuleID::R_METHOD: return "R_METHOD";
	case RuleID::R_VAR:    return "R_VAR";
	case RuleID::R_ACCESS: return "R_ACCESS";
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

