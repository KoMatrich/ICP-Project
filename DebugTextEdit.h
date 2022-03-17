#pragma once

#include <QTextEdit>
#include "highlighter.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

class DebugTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit DebugTextEdit(QWidget *parent = 0);
signals:

public slots:

protected:

private:
    Highlighter *syntax;
};
