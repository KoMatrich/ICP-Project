#include <QtWidgets>
#include "TextCode.h"

MainTextEdit::MainTextEdit(QWidget *parent) : QTextEdit(parent)
{
    highlighter = new Highlighter(this);
}

void MainTextEdit::syntax_reload(){
    highlighter->rehighlight();
};
