#pragma once

#include <QWidget>
#include "Graph/SEQItem.h"
#include "Semantics.h"

class SEQScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SEQScene(QObject* parent = nullptr);

public slots:
    void update();
private:
    void add(UMLClass const clas);
    void rem(uint index);
};

class SEQView : public QGraphicsView
{
    Q_OBJECT
public:
    SEQView(QObject* parent = nullptr);
    SEQScene* getScene() { return &scene; }
public slots:
    void update();
private:
    SEQScene scene;
};
