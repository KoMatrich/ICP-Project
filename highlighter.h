#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QTextEdit>

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextEdit *parent = 0);

protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
    struct Rule
    {
        QRegExp start;
        QRegExp end;
        QTextCharFormat format;

        QVector<Rule> parts;
    };
    typedef QVector<Rule> RuleSet;

    typedef QVector<int> Path;
    typedef QVector<Path> PathStack;

    PathStack path_stack;
    RuleSet syntax;

    QTextCharFormat err;
    QTextCharFormat after_err;
    QTextCharFormat cursor_color;

    void inline error(int code, const QString msg);
    void getRules(Rule &current,RuleSet &parts, Path *path);
    int match(const QString &text, int &offset, Path *path);
    void find(const QString &text, int line, int &offset);
};
