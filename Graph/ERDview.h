#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

class ERDview : public QWidget
{
    Q_OBJECT
public:
    explicit ERDview(QTextEdit *parent = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTextEdit *editor;
};
