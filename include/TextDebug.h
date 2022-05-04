#pragma once

#include <QTextEdit>

/// @brief Debug console
class DebugTextEdit : public QTextEdit
{

public:
    /// @brief          creates debug console read-only
    /// @param parent   parent widget
    explicit DebugTextEdit(QWidget* parent = 0);
protected:

private:
};
