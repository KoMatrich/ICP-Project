#pragma once

#include <QSyntaxHighlighter>
#include <QDebug>
#include "Semantics.h"

/// @brief Syntax Highlighter
class Highlighter : public QSyntaxHighlighter
{
public:
    /// @brief              creates new highlighter
    /// @param parent       text editor to highlight
    Highlighter(QTextEdit* parent);
protected:
    /// @brief          Highlighting function
    /// @param text     line of file
    void highlightBlock(const QString& text) override;
private:
    /// @brief          pointer to syntax tree
    SyntaxTree* syntax_tree;
    /// @brief          pointer to syntax analyzer
    Analyzer* analyzer;
    /// @brief updates index of current line
    /// @note indexing is also used for updating
    void updateCurrentBlockState();
    /// @brief Skips to next non white space char on line
    void skipSpace(const QString text, int& offset);
};
