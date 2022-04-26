#include <QtWidgets>
#include "TextDebug.h"

DebugTextEdit::DebugTextEdit(QWidget *parent) : QTextEdit(parent)
{
    this->setReadOnly(true);
    this->printText("Write some code or open a file to get started...");
}

void DebugTextEdit::printText(QString msg)
{
    this->append(msg);
}

void DebugTextEdit::clearText()
{
    this->clear();
}
