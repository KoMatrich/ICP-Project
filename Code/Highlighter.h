#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextEdit>
#include <QDebug>
#include "syntax.h"

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextEdit *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    typedef QVector<int> Path;
    typedef QVector<Path> PathStack;

    PathStack path_stack;

    QTextCharFormat cursor_color;
    QTextCharFormat err;
    QTextCharFormat after_err;

    void getRules(Rule &current,RuleSet &parts, Path *path);
    int match(const QString &text, int &offset, Path *path);
    void find(const QString &text, int line, int &offset);
    void inline error(int code, const QString msg);
};
