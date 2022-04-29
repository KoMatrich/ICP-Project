#include "CodeService.h"

void CodeService::setEndpoint(MainTextEdit* c)
{
    CodeService::getInstance()->setCodeWindow(c);
}

void CodeService::clearBackground()
{
    MainTextEdit* editor = CodeService::getInstance()->code;
    QTextCursor cursor = editor->textCursor();
    QTextBlockFormat f;
    f.setBackground(Qt::transparent);
    cursor.select(QTextCursor::Document);
    cursor.setBlockFormat(f);

}

void CodeService::formatLine(size_t ln)
{
    ln = 5;
    MainTextEdit* editor = CodeService::getInstance()->code;
    QTextCursor cursor = QTextCursor(editor->document()->findBlockByLineNumber(ln - 1));
    QTextBlockFormat f;
    f.setBackground(QColor(250,165,165));
    cursor.select(QTextCursor::LineUnderCursor);
    cursor.setBlockFormat(f);
}

void CodeService::setCodeWindow(MainTextEdit* c)
{
	this->code = c;
}
