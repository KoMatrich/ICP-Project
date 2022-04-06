#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextEdit>
#include <QDebug>
#include "Syntax.h"

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextEdit *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;
    void updateCurrentBlockState();

private:
    typedef QVector<int> Path;
    typedef QVector<Path> PathStack;

    PathStack path_stack;

    Syntax syntax;

    enum LineState{NORMAL,FILE_START=-1,SYNTAX_E=-2,INTERNAL_E=-3};

    void getRules(Rule &current,RuleSet &parts, Path *path);
    int match(const QString &text, int &offset, Path *path);
    void find(const QString &text, int line, int &offset);
    void inline error(int code, const QString msg);
};
