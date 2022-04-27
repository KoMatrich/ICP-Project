#pragma once

#include <QTextEdit>
#include "Highlighter.h"
#include "TextDebug.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

class MainTextEdit : public QTextEdit
{
	Q_OBJECT
public:
	MainTextEdit(QWidget* parent = 0) : QTextEdit(parent)
	{
		highlighter = new Highlighter(this);
	}
signals:

public slots:
	void syntax_reload() {
		highlighter->rehighlight();
	};
protected:

private:
	Highlighter* highlighter;
};
