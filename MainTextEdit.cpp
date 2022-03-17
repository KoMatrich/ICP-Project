#include <QtWidgets>
#include "MainTextEdit.h"

MainTextEdit::MainTextEdit(QWidget *parent)
{
    this->setTextBackgroundColor(QColor(220,250,250));
    this->setParent(parent);

    highlighter = new Highlighter(this->document());
}
