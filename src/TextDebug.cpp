#include <QtWidgets>
#include "TextDebug.h"

DebugTextEdit::DebugTextEdit(QWidget* parent) : QTextEdit(parent)
{
	this->setReadOnly(true);
}
