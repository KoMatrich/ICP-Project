#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextEdit>
#include <QDebug>
#include "Syntax/Analyzer.h"
#include "TextDebug.h"
#include "Services/Highlight.h"
#include "Semantics.h"

class Highlighter : public QSyntaxHighlighter
{
public:
    Highlighter(QTextEdit* parent);

protected:
    void highlightBlock(const QString& text) override;
    void updateCurrentBlockState();

private:
    SyntaxTree* syntax;
    Analyzer* analyzer;
    void skipSpace(const QString text, int& offset);
};
