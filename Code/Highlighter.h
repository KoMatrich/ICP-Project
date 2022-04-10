#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextEdit>
#include <QDebug>
#include "Analyzer.h"

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextEdit *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;
    void updateCurrentBlockState();

private:
    SyntaxTree* syntax;
    Analyzer* analyzer;
};
