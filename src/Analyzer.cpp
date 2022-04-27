#include "Analyzer.h"
#include "QDebug"
#include <iostream>
#include <format>
#include "DebugService.h"


/// Returns current rule on top of line stack
/// without poping inline rules
void inline Analyzer::getRules(Rule& current, RuleSet& parts, LineStack stack) {
	parts = syntax_tree->getRules();
	current = Rule();
	current.type = RuleType::MULTI_LINE;

	if (stack.length() != 0) {
		current = stack.back().first;
		parts = current.parts;
	}
}

///If found index of matched rule, else -1
Lexem* Analyzer::matchBody(const QString& text, int& offset, RuleSet parts)
{
	//find start of sub block
	for (int i = 0; i < parts.length(); i++) {
		Rule part = parts.at(i);
		QRegExp start = part.start;
		int match_i = start.indexIn(text, offset);
		if (match_i == offset) {
			Lexem* lex = new Lexem();
			lex->first = part;
			lex->second = text.mid(offset, start.matchedLength()).toLatin1();

			//VitaPrintf("Matched:part %1 d / %2 d.start", VF(i)VF(parts.length()));
			//VitaPrint("\t" + lex->second);
			//VitaPrint("\t" + part.start.pattern());

			offset += start.matchedLength();
			return lex;
		}
	}

	//no rule was matched
	return nullptr;
}

///If found returns 0, else -1
int Analyzer::matchEnd(const QString& text, int& offset, Rule current)
{
	//find end of current block
	QRegExp end = current.end;
	if (!end.isEmpty()) {
		int match_i = end.indexIn(text, offset);
		if (match_i == offset) {
			//VitaPrint("Matched:body.end");
			//VitaPrint("\t" + text.mid(offset, end.matchedLength()));
			//VitaPrint("\t" + current.end.pattern());

			offset += end.matchedLength();
			return 0;
		}
	}
	else {
		//block without ending
		//VitaPrint("Ending :body.end");
		//VitaPrint("\t Empty patern");

		return 1;
	}

	//no rule was matched
	return -1;
}

/// Pops INLINE rules from LineStack
void Analyzer::reduceStack(LineStack* stack)
{
	while (stack->length() != 0) {
		Rule current = stack->back().first;

		if (current.end.isEmpty() && current.type != RuleType::MULTI_LINE) {
			stack->removeLast();
		}
		else {
			break;
		}
	}
}

/// Finds next lexem in text and updates LineStack.
/// Offset is lengh of match, if lexem is found.
/// Offset is unchenged on empty rule match
/// (used as last rule when matching)
void Analyzer::Next(int line, int& offset, const QString& text, Rule& current)
{
	LineStack* stack;
	RuleSet parts;

	if (global_stack.length() == line + 1) {
		//recursive call
		stack = new LineStack(global_stack.at(line));
	}
	else {
		//first call of this function on this line
		if (line == 0) {
			//first line
			stack = new LineStack();
		}
		else {
			//load stack from previous line
			stack = new LineStack(global_stack.at(line - 1));
			reduceStack(stack);
		}
	}

	getRules(current, parts, *stack);
	Lexem* lex = nullptr;
	int result;

	if ((offset == 0) && (current.type == RuleType::INLINE)) {
		offset = SYNTAX_E;
		//qDebug() << "Previus rule didn't end well";
		goto SKIP;
	}

	if (offset == text.length())
		goto SKIP;

	lex = matchBody(text, offset, parts);
	if (lex != nullptr) {
		stack->append(*lex);
		free(lex);
		getRules(current, parts, *stack);
		goto SKIP;
	}

	if (stack->size() == 0) {
		//not in body so no checking
		offset = NO_CHECK;
		goto SKIP;
	}

	result = matchEnd(text, offset, current);
	if (result >= 0) {
		stack->removeLast();
		goto SKIP;
	}

	//nothing found but text is still remaining
	offset = SYNTAX_E;
SKIP:
	ClearTo(line);
	global_stack.append(*stack);
}

/// Removes all LineStacks with index > line number
/// from GlobalStack
void Analyzer::ClearTo(int lineNumber)
{
	while (lineNumber < global_stack.length()) {
		global_stack.removeLast();
	}
}

void Analyzer::ClearAll()
{
	global_stack.clear();
}

GlobalStack Analyzer::GetStack()
{
	return this->global_stack;
}

