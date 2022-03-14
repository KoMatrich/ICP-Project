#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

class GraphView : public QWidget
{
    Q_OBJECT
public:
    explicit GraphView(QWidget *parent = 0,QTextEdit *editor = 0);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QTextEdit *editor;
};

#endif // GRAPHVIEW_H
