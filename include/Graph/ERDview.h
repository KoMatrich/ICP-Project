#pragma once

#include <QWidget>
#include <Graph/ERDItem.h>

class ERDScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ERDScene(QObject* parent = nullptr);

    void update();

public slots:
    void documentWasModified();
};

class ERDView : public QGraphicsView
{
    Q_OBJECT
public:
    ERDView(QObject* parent = nullptr);
    ERDScene* getScene() { return &scene; }
private:
    ERDScene scene;
};
