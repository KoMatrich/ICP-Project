#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

class SEQview : public QWidget
{
    Q_OBJECT
public:
    explicit SEQview(QTextEdit *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTextEdit *editor;
};
