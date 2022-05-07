/**
 * @file Rules.h
 *
 * @brief Contains rules types declarations and structures
 *
 * @author Martin Kocich
 * Contact: xkocic02@stud.fit.vutbr.cz
 *
 * @author Vitezslav Kriz
 * Contact xkrizv03@stud.fit.vutbr.cz
 *
 */

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
    R_SEQ, R_ACTIVATE, R_DEACTIVATE, R_ARROW_SYNC, R_ARROW_ASYNC, R_ARROW_CALLBACK, R_COLON, R_SEQUENCE, R_SEQUENCENAME, R_SEQUENCEBLOCK, R_NOP, R_NOPSIZE

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
    case RuleID::R_ASS: return "association"; break;
    case RuleID::R_AGG: return "aggregation"; break;
    case RuleID::R_COM: return "composition"; break;
    case RuleID::R_GEN: return "generalization"; break;
    case RuleID::R_XPOS: return "R_XPOS"; break;
    case RuleID::R_YPOS: return "R_YPOS"; break;
    case RuleID::R_POSVAL: return "R_POSVAL"; break;
    case RuleID::R_SEQ: return "R_SEQ"; break;
    case RuleID::R_ACTIVATE: return "R_ACTIVATE"; break;
    case RuleID::R_DEACTIVATE: return "R_DEACTIVATE"; break;
    case RuleID::R_ARROW_SYNC: return "R_ARROW_SYNC"; break;
    case RuleID::R_ARROW_ASYNC: return "R_ARROW_ASYNC"; break;
    case RuleID::R_ARROW_CALLBACK: return "R_ARROW_CALLBACK"; break;
    case RuleID::R_COLON: return "R_COLON"; break;
    case RuleID::R_SEQUENCE: return "R_SEQUENCE"; break;
    case RuleID::R_SEQUENCENAME: return "R_SEQUENCENAME"; break;
    case RuleID::R_SEQUENCEBLOCK: return "R_SEQUENCEBLOCK"; break;
    case RuleID::R_NOP: return "R_NOP"; break;
    case RuleID::R_NOPSIZE: return "R_NOPSIZE"; break;

    default:      return "[Unknown]";
    }
}

/// @brief Structure that stores rule data
struct Rule
{
    /// @brief rule starting regex
    QRegExp start;
    /// @brief rule ending regex
    QRegExp end;
    /// @brief rule formating
    QTextCharFormat format;
    /// @brief rule type
    RuleType type = RuleType::INLINE;
    /// @brief rule syntax id
    RuleID id = RuleID::R_ERR;
    /// @brief rule whitespace requirement
    RuleWhitespace wh = RuleWhitespace::W_REQUIRED;
    /// @brief rule inner parts
    std::vector<Rule*> parts;
};

/// @brief stores pointer to syntax tree rules
typedef std::vector<Rule*> RuleSet;

/// @brief Main syntax tree
class SyntaxTree
{
public:
    SyntaxTree();
    /// @brief syntax error formating
    QTextCharFormat err;
    /// @brief cursor formating
    QTextCharFormat cursor_color;
    /// @brief formating after syntax error
    /// @note  this text is not checked by syntax any more
    QTextCharFormat after_err;
    /// @brief text that is ignored
    QTextCharFormat no_check;
    /// @brief Format of positional descriptor
    static QTextCharFormat posFormat()
    {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        return format;
    };
    /// @brief Format of positional value
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
    /// @brief main syntax tree after is generated
    RuleSet syntax_tree;
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

