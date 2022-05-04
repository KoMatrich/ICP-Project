#pragma once

#include <QTextCharFormat>

enum class RuleType
{
    INLINE,     /// is poped from stack 
    MULTI_LINE  /// MUST match end
};

enum class RuleWhitespace
{
    W_REQUIRED,
    W_OPTIONAL
};

enum class RuleID
{
    R_ERR = -1, R_UML, R_CLASS, R_INTERFACE, R_ENTITYBLOCK, R_ENTITYNAME, R_METHOD, R_ATTRIBUTE, R_TYPE, R_ACCESS, R_IN, R_ASS, R_AGG, R_COM, R_GEN, R_XPOS, R_YPOS, R_POSVAL,
    R_SEQ, R_ACTIVATE, R_DEACTIVATE, R_ARROW_SYNC, R_ARROW_ASYNC, R_COLON

};

inline const char* RuleIDtoString(RuleID r)
{
    switch (r) {
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
    case RuleID::R_XPOS: return "R_XPOS"; break;
    case RuleID::R_YPOS: return "R_YPOS"; break;
    case RuleID::R_POSVAL: return "R_POSVAL"; break;
    case RuleID::R_SEQ: return "R_SEQ"; break;
    case RuleID::R_ACTIVATE: return "R_ACTIVATE"; break;
    case RuleID::R_DEACTIVATE: return "R_DEACTIVATE"; break;
    case RuleID::R_ARROW_SYNC: return "R_ARROW_SYNC"; break;
    case RuleID::R_ARROW_ASYNC: return "R_ARROW_ASYNC"; break;
    case RuleID::R_COLON: return "R_COLON"; break;

    default:      return "[Unknown]";
    }
}

/// @brief Structure that stores rule data
struct Rule
{
    QRegExp start;
    QRegExp end;
    QTextCharFormat format;
    RuleType type = RuleType::INLINE;
    RuleID id = RuleID::R_ERR;
    RuleWhitespace wh = RuleWhitespace::W_REQUIRED;
    std::vector<Rule*> parts;
};

/// @brief stores pointer to syntax tree rules
typedef std::vector<Rule*> RuleSet;

/// @brief Main syntax tree
class SyntaxTree
{
public:
    SyntaxTree();
    QTextCharFormat err, cursor_color, after_err, no_check;
    static QTextCharFormat posFormat()
    {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        return format;
    };
    static QTextCharFormat posValFormat()
    {
        QTextCharFormat format;
        format.setForeground(Qt::darkMagenta);
        return format;
    };
    /// @return Pointers to syntax tree
    RuleSet const getRules();
private:
    /// @brief Generates syntax tree rules
    RuleSet const genRules();
    RuleSet syntax;
};

/// @brief Syntax state
enum SYNTAX_STATE
{
    NO_CHECK = -1,
    SYNTAX_ERR = -2
};

/// @brief State of line for syntax highlighter
enum LINE_STATE
{
    FILE_START = -1,
    SYNTAX_E = -2,
    INTERNAL_E = -3
};

