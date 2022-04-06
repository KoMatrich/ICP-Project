#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
class QTextEdit;
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

class ERDview : public QWidget
{
    Q_OBJECT
public:
    explicit ERDview(QTextEdit *parent = 0);

signals:

public slots:

protected:
    void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    QTextEdit *editor;
};
