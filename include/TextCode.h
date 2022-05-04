#pragma once
#include "Syntax/Highlighter.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

/// @brief Text editor
class MainTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    /// @brief              Creates new text editor
    /// @param parent       parent QWidget
    MainTextEdit(QWidget* parent = 0) : QTextEdit(parent)
    {
        highlighter = new Highlighter(this);
    }
public slots:
    /// @brief Does syntax reload
    void syntax_reload()
    {
        highlighter->rehighlight();
    };
private:
    /// @brief text editor highlighter
    Highlighter* highlighter;
};
