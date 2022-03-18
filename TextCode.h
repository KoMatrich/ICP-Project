#pragma once

#include <QTextEdit>
#include "Highlighter.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

class MainTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit MainTextEdit(QWidget *parent = 0);
signals:

public slots:
    void syntax_reload();
protected:

private:
    Highlighter *highlighter;
};
