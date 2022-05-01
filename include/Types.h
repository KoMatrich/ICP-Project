#pragma once

#include "Syntax/Rules.h"
#include <QString>

typedef std::pair<Rule*, QString> Lexem;
typedef std::vector<Lexem> LineStack;
typedef std::vector<LineStack> GlobalStack;
