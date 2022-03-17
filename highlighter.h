#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent = 0);

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

    QVector<int> stack;
    RuleSet syntax;

    QTextCharFormat err;
    QTextCharFormat after_err;

    int match(const QString &text, int &offset, RuleSet &parts, Rule &current);
    void find(const QString &text, int &offset);
};

/*
//simple one line rules
foreach (const HighlightingRule &rule, highlightingRules) {
    QRegExp expression(rule.pattern);
    int index = expression.indexIn(text);
    while (index >= 0) {
        int length = expression.matchedLength();
        setFormat(index, length, rule.format);
        index = expression.indexIn(text, index + length);
    }
}

//simple multi line rule
setCurrentBlockState(0);

int startIndex = 0;
if (previousBlockState() != 1)
    startIndex = commentStartExpression.indexIn(text);

while (startIndex >= 0) {
    int endIndex = commentEndExpression.indexIn(text, startIndex);
    int commentLength;
    if (endIndex == -1) {
        setCurrentBlockState(1);
        commentLength = text.length() - startIndex;
    } else {
        commentLength = endIndex - startIndex
                        + commentEndExpression.matchedLength();
    }
    setFormat(startIndex, commentLength, multiLineCommentFormat);
    startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
}
    //key words
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);

    QStringList keywordPatterns;
    keywordPatterns << "^participant" << "^'participant" << "^title"
                    << "^class"       << "^bactivate"   << "^note"
                    << "^deactivate"  << "^activate";

    HighlightingRule rule;
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    //@key words
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("^@[A-Za-z]+");
    rule.format = classFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\"[^\"]*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("^[A-Za-z0-9_]+ ?(\\-\\>|\\-\\-\\>) ?[A-Za-z0-9_]+:");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegExp("^' [^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::gray);
    commentStartExpression = QRegExp("^/' ");
    commentEndExpression = QRegExp(" '/$");
*/
