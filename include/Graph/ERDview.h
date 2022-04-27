#pragma once

#include <QWidget>
#include <Graph/ERDItem.h>

class ERDview : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ERDview(QObject *parent = nullptr);
    
    void update();

public slots:
    void documentWasModified();
};
