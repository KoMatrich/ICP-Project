#pragma once

#include <QTextEdit>

class QTextEdit;

class DebugTextEdit : public QTextEdit
{

public:
    explicit DebugTextEdit(QWidget *parent = 0);
    void printText(QString msg);
    void clearText();
protected:

private:
};
