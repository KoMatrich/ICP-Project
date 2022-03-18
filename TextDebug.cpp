#include <QtWidgets>
#include "TextDebug.h"

DebugTextEdit::DebugTextEdit(QWidget *parent) : QTextEdit(parent)
{
    this->setTextBackgroundColor(QColor(250,220,250));
}
