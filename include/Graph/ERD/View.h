/**
 * @file View.h
 *
 * @brief Contains declaration of view for ERD
 *
 * @author Martin Kocich
 * Contact: xkocic02@stud.fit.vutbr.cz
 *
 */

#pragma once

#include <QWidget>
#include "Graph/ERD/Item.h"
#include "Graph/ERD/Arrow.h"

 /// @brief ERD scene
class ERDScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /// @brief          creates new ERD scene
    /// @param parent   parent QObject
    explicit ERDScene(QObject* parent = nullptr);
    /// @brief          updates scene
    void update();
private:
    /// @brief          adds classes to scene
    /// @param classes  classes 
    void addClasses(std::vector<UMLClass> classes);
    /// @brief          adds arrows to scene
    /// @param classes  classes with relations
    void addArrows(std::vector<UMLClass> classes);
protected:
    /// @brief          handles code refreshing after dragging
    /// @param event    mouse release
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
    {
        QGraphicsScene::mouseReleaseEvent(event);
    }
};

/// @brief ERD view
class ERDView : public QGraphicsView
{
    Q_OBJECT
public:
    /// @brief          creates new ERD view
    /// @param parent   parent QObject
    ERDView(QObject* parent = nullptr);
    /// @brief          gets scene from view
    /// @return         scene
    ERDScene* getScene() { return &scene; }
public slots:
    /// @brief          triggers scene update
    void update() { scene.update(); };
private:
    /// @brief          scene of this view
    ERDScene scene;

};
