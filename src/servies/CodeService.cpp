#include "services/CodeService.h"

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

void CodeService::updatePos(size_t entity_ln, size_t x_ln, int x_val, size_t y_ln, int y_val)
{
    HighlightService::setEnabled(false);
    QTextEdit* editor = CodeService::getInstance()->code;

    if (x_ln > 0)
    {
        QTextBlock block = editor->document()->findBlockByLineNumber(x_ln);
        //str.left(std.indexOf("something") + 9)
        QString prefix = block.text().left(block.text().indexOf("x"));
        QTextCursor cursor = QTextCursor(block);
    }

    //QTextCursor cursor = QTextCursor(editor->document()->findBlockByLineNumber(ln));
    HighlightService::setEnabled(true);
}


void CodeService::insertLine(size_t ln, QString text)
{
    QTextEdit* editor = CodeService::getInstance()->code;
    QTextCursor cursor = QTextCursor(editor->document()->findBlockByLineNumber(ln));
    cursor.insertText(text);
}

void CodeService::setCodeWindow(QTextEdit* c)
{
    this->code = c;
}
