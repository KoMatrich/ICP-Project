#include <QtWidgets>
#include "DebugTextEdit.h"

DebugTextEdit::DebugTextEdit(QWidget *parent)
{
    this->setTextBackgroundColor(QColor(250,220,250));
    this->setParent(parent);

    syntax = new Highlighter(this->document());
}
