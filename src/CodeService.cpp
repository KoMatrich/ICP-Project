#include "CodeService.h"

void CodeService::setEndpoint(MainTextEdit* c)
{
    CodeService::getInstance()->setCodeWindow(c);
}

void CodeService::formatLine(size_t ln)
{
    MainTextEdit* editor = CodeService::getInstance()->code;
    QTextCursor cur = editor->textCursor();
    QTextBlockFormat f;
    f.setBackground(QColor(250,165,165));
    cur.select(QTextCursor::LineUnderCursor);
    cur.setBlockFormat(f);
}

void CodeService::setCodeWindow(MainTextEdit* c)
{
	this->code = c;
}
