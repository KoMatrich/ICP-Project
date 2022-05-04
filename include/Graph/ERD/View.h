#pragma once

#include <QWidget>
#include "Graph/ERD/Item.h"
#include "Graph/ERD/Arrow.h"
#include "Semantics.h"
#include "Services/Code.h"

class ERDScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit ERDScene(QObject* parent = nullptr);
    void update();
private:
    void add(UMLClass const clas);
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
    {

        CodeService::callCachedUpdatePos();
        
        QGraphicsScene::mouseReleaseEvent(event);
    }
};

class ERDView : public QGraphicsView
{
    Q_OBJECT
public:
    ERDView(QObject* parent = nullptr);
    ERDScene* getScene() { return &scene; }
public slots:
    void update() { scene.update(); };
private:
    ERDScene scene;

};
