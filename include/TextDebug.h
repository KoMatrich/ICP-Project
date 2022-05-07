/**
 * @file TextDebug.h
 *
 * @brief Contains declaration of main debug window
 *
 * @author Martin Kocich
 * Contact: xkocic02@stud.fit.vutbr.cz
 *
 * @author Vitezslav Kríz
 * Contact xkrizv03@stud.fit.vutbr.cz
 *
 */

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
