#include <QtWidgets>
#include "TextCode.h"

MainTextEdit::MainTextEdit(QWidget *parent) : QTextEdit(parent)
{
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    this->setFont(font);

    const int tabStop = 3;  //characters
    QFontMetrics metrics(font);
    this->setTabStopWidth(tabStop * metrics.width(' '));

    highlighter = new Highlighter(this);
}

void MainTextEdit::syntax_reload(){
    highlighter->rehighlight();
};
