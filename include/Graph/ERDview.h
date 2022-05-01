#pragma once

#include <QWidget>
#include "Graph/ERDItem.h"
#include "Semantics.h"

class ERDScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ERDScene(QObject* parent = nullptr);

public slots:
    void update();
private:
    void add(UMLClass const clas);
    void rem(uint index);
};

class ERDView : public QGraphicsView
{
    Q_OBJECT
public:
    ERDView(QObject* parent = nullptr);
    ERDScene* getScene() { return &scene; }
public slots:
    void update();
private:
    ERDScene scene;
};
