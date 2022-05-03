#pragma once

#include <QTextEdit>
#include "Syntax/Highlighter.h"
#include "TextDebug.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

/// @brief Text editor
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
    /// @brief Does syntax reload
    void syntax_reload()
    {
        highlighter->rehighlight();
    };
private:
    Highlighter* highlighter;
};
