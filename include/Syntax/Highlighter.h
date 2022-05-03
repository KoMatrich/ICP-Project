#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextEdit>
#include <QDebug>
#include "Syntax/Analyzer.h"
#include "TextDebug.h"
#include "Services/Highlight.h"
#include "Semantics.h"

/// @brief Syntax Highlighter
class Highlighter : public QSyntaxHighlighter
{
public:
    Highlighter(QTextEdit* parent);

protected:
    /// @brief          Highlighting function
    /// @param text     line of file
    void highlightBlock(const QString& text) override;
private:
    SyntaxTree* syntax;
    Analyzer* analyzer;
    /// @brief updates index of current line
    /// @note indexing is also used for updating
    void updateCurrentBlockState();
    /// @brief Skips to next non white space char on line
    void skipSpace(const QString text, int& offset);
};
