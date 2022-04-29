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

inline const char* RuleIDtoString(RuleID r)
{
	switch (r)
	{
    case RuleID::R_ERR: return "R_ERR"; break;
    case RuleID::R_UML: return "R_UML"; break;
    case RuleID::R_CLASS: return "R_CLASS"; break;
    case RuleID::R_INTERFACE: return "R_INTERFACE"; break;
    case RuleID::R_ENTITYBLOCK: return "R_ENTITYBLOCK"; break;
    case RuleID::R_ENTITYNAME: return "R_ENTITYNAME"; break;
    case RuleID::R_METHOD: return "R_METHOD"; break;
    case RuleID::R_ATTRIBUTE: return "R_ATTRIBUTE"; break;
    case RuleID::R_TYPE: return "R_TYPE"; break;
    case RuleID::R_ACCESS: return "R_ACCESS"; break;
    case RuleID::R_IN: return "R_IN"; break;
    case RuleID::R_ASS: return "R_ASS"; break;
    case RuleID::R_AGG: return "R_AGG"; break;
    case RuleID::R_COM: return "R_COM"; break;
    case RuleID::R_GEN: return "R_GEN"; break;

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
    std::vector<Rule *> parts;
};

typedef std::vector<Rule *> RuleSet;

class SyntaxTree {
public:
	SyntaxTree();
	QTextCharFormat err, cursor_color, after_err, no_check;

	RuleSet const getRules();
private:
	RuleSet const genRules();
    RuleSet syntax;
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

