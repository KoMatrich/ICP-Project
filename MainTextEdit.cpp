#include <QtWidgets>
#include "MainTextEdit.h"

MainTextEdit::MainTextEdit(QWidget *parent)
{
    this->setParent(parent);

    highlighter = new Highlighter(this);
}
