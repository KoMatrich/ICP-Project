/**
 * @file Types.h
 *
 * @brief Contains typedefs for semantics
 *
 * @author Vitezslav Kríz
 * Contact xkrizv03@stud.fit.vutbr.cz
 *
 */

#pragma once

#include "Syntax/Rules.h"
#include <QString>

 /// @brief Pair(rule pointer,matched lexem)
typedef std::pair<Rule*, QString> Lexem;
/// @brief Stack of rules for specific line
typedef std::vector<Lexem> LineStack;
/// @brief Stack of LineStack
typedef std::vector<LineStack> GlobalStack;
