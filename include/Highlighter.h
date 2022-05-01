#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextEdit>
#include <QDebug>
#include "Analyzer.h"
#include "TextDebug.h"
#include "services/HighlightService.h"
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
