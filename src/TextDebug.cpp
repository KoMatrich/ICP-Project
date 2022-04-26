#include <QtWidgets>
#include "TextDebug.h"

DebugTextEdit::DebugTextEdit(QWidget *parent) : QTextEdit(parent)
{
    this->setReadOnly(true);
    this->append("Write some code or open a file to get started...");
}