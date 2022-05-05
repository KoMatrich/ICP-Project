#include "Services/Code.h"
#include "Syntax/Rules.h"
#include "Services/History.h"

void CodeService::setEndpoint(QTextEdit* c)
{
    CodeService::getInstance().setCodeWindow(c);
}

void CodeService::clearBackground()
{
    QTextEdit* editor = CodeService::getInstance().code;
    QTextCursor cursor = editor->textCursor();
    QTextBlockFormat f;
    f.setBackground(Qt::transparent);
    cursor.select(QTextCursor::Document);
    cursor.setBlockFormat(f);

}

void CodeService::deleteEntity(size_t start, size_t end)
{
    HistoryService::takeHistorySnapshot(); //before change
    QTextEdit* editor = CodeService::getInstance().code;
    QTextCursor cursor = QTextCursor(editor->document()->findBlockByLineNumber(start));
    cursor.movePosition(QTextCursor::Down, QTextCursor::KeepAnchor, end - start + 1);
    cursor.removeSelectedText();
}

void CodeService::highlightClass(size_t ln)
{
    HighlightService::setEnabled(false);
    CodeService::formatLine(ln, HLevel::LEVEL_OK);
    QTextEdit* editor = CodeService::getInstance().code;
    QTextCursor cursor = QTextCursor(editor->document()->findBlockByLineNumber(ln));
    editor->setTextCursor(cursor);
    HighlightService::setEnabled(true);
}

void CodeService::relationRequest(QString sel_name, RuleID rule, size_t line)
{
    CodeService& instance = CodeService::getInstance();
    QTextEdit* editor = instance.code;
    editor->setDisabled(true);
    instance.isConnecting = true;
    instance.sel_name = sel_name;
    instance.rule = rule;
    instance.rel_line = line;
}

void CodeService::relationConnect(QString target)
{
    CodeService& instance = CodeService::getInstance();
    if (!instance.isConnecting) return;

    HistoryService::takeHistorySnapshot(); //before change
    instance.isConnecting = false;
    QTextEdit* editor = instance.code;
    QTextBlock block = editor->document()->findBlockByLineNumber(instance.rel_line);
    QTextCursor cursor = QTextCursor(block);
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.insertText(QString("   in ") + RuleIDtoString(instance.rule) + QString(" with ") + target + QString("\n"));
    editor->setDisabled(false);
}

void CodeService::formatLine(size_t ln, HLevel level)
{
    QTextEdit* editor = CodeService::getInstance().code;
    QTextCursor cursor = QTextCursor(editor->document()->findBlockByLineNumber(ln));
    QTextBlockFormat f;
    switch (level) {
    case HLevel::LEVEL_OK:
        f.setBackground(QColor(226, 255, 191));
        break;
    case HLevel::LEVEL_WARN:
        f.setBackground(QColor(255, 225, 185));
        break;
    case HLevel::LEVEL_ERROR:
        f.setBackground(QColor(250, 165, 165));
        break;
    default:
        break;
    }
    cursor.select(QTextCursor::LineUnderCursor);
    cursor.setBlockFormat(f);
}

// TODO optimize to use less inserts
void CodeService::updatePos(size_t entity_ln, size_t x_ln, int x_val, size_t y_ln, int y_val)
{
    if (!CodeService::getInstance().isPosActive)
        return;
    HistoryService::takeHistorySnapshot(); //before change

    //disables syntax for changes to prevent recursion
    HighlightService::setEnabled(false);
    QTextEdit* editor = CodeService::getInstance().code;
    QTextCursor cursor;
    QString prefix = "";
    QTextBlock block;
    //Y
    if (y_ln > 0) {
        block = editor->document()->findBlockByLineNumber(y_ln);
        prefix = block.text().left(block.text().indexOf("y"));
        cursor = QTextCursor(block);
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.insertText(" ");
        cursor.movePosition(QTextCursor::Left);
        cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
    } else {
        block = editor->document()->findBlockByLineNumber(entity_ln + 1);
        cursor = QTextCursor(block);
        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.insertText("\n");
        cursor.movePosition(QTextCursor::Up);
        HighlightService::setEnabled(true);
    }
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.setCharFormat(SyntaxTree::posFormat());
    cursor.insertText(prefix + "y:");
    if (!(y_ln > 0)) cursor.insertText(" ");
    cursor.movePosition(QTextCursor::EndOfLine);
    cursor.setCharFormat(SyntaxTree::posValFormat());
    cursor.insertText(QString::number(y_val));
    // X
    if (x_ln > 0) {
        block = editor->document()->findBlockByLineNumber(x_ln);
        prefix = block.text().left(block.text().indexOf("x"));
        cursor = QTextCursor(block);
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.insertText(" ");
        cursor.movePosition(QTextCursor::Left);
        cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
        cursor.removeSelectedText();
    } else {
        block = editor->document()->findBlockByLineNumber(entity_ln + 1);
        cursor = QTextCursor(block);
        cursor.movePosition(QTextCursor::StartOfLine);
        cursor.insertText("\n");
        cursor.movePosition(QTextCursor::Up);
        HighlightService::setEnabled(true);
    }
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.setCharFormat(SyntaxTree::posFormat());
    cursor.insertText(prefix + "x:");
    if (!(x_ln > 0)) cursor.insertText(" ");
    cursor.movePosition(QTextCursor::EndOfLine);
    cursor.setCharFormat(SyntaxTree::posValFormat());
    //enables syntax to trigger update
    HighlightService::setEnabled(true);
    cursor.insertText(QString::number(x_val));
}

void CodeService::cacheUpdatePos(size_t entity_ln, size_t x_ln, int x_val, size_t y_ln, int y_val)
{
    CodeService& instance = CodeService::getInstance();
    instance.new_class_line = entity_ln;
    instance.new_x_line = x_ln;
    instance.new_x = x_val;
    instance.new_y_line = y_ln;
    instance.new_y = y_val;
}

void CodeService::insertLine(size_t ln, QString text)
{
    HistoryService::takeHistorySnapshot(); //before change
    QTextEdit* editor = CodeService::getInstance().code;
    QTextCursor cursor = QTextCursor(editor->document()->findBlockByLineNumber(ln));
    cursor.insertText(text);
}

void CodeService::setPosActive(bool val)
{
    CodeService& instance = CodeService::getInstance();
    instance.isPosActive = val;
}


void CodeService::callCachedUpdatePos()
{
    CodeService& instance = CodeService::getInstance();
    CodeService::updatePos(instance.new_class_line, instance.new_x_line, instance.new_x, instance.new_y_line, instance.new_y);
}

void CodeService::setCodeWindow(QTextEdit* c)
{
    this->code = c;
}
