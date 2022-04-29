#include "CodeService.h"

void CodeService::setEndpoint(QTextEdit* c)
{
    CodeService::getInstance()->setCodeWindow(c);
}

void CodeService::clearBackground()
{
    QTextEdit* editor = CodeService::getInstance()->code;
    QTextCursor cursor = editor->textCursor();
    QTextBlockFormat f;
    f.setBackground(Qt::transparent);
    cursor.select(QTextCursor::Document);
    cursor.setBlockFormat(f);

}

void CodeService::formatLine(size_t ln, HLevel level)
{
    QTextEdit* editor = CodeService::getInstance()->code;
    QTextCursor cursor = QTextCursor(editor->document()->findBlockByLineNumber(ln));
    QTextBlockFormat f;
    switch (level)
    {
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

void CodeService::setCodeWindow(QTextEdit* c)
{
	this->code = c;
}
