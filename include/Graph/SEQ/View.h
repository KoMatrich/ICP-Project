#pragma once

#include <QWidget>
#include "Graph/SEQ/Column.h"

/// @brief SEQ scene
class SEQScene : public QGraphicsScene
{
    Q_OBJECT
public:
    /// @brief          creates new SEQ scene
    /// @param parent   parent QObject
    explicit SEQScene(QObject* parent = nullptr);
    /// @brief          updates scene
    void update();
private:
    /// @brief          adds column to scene
    /// @param classes  classes 
    void addColumn(std::vector<UMLClass> colums);
protected:
    /// @brief          handles code refreshing after dragging
    /// @param event    mouse release
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
    {
        QGraphicsScene::mouseReleaseEvent(event);
    }
};

/// @brief ERD view
class SEQView : public QGraphicsView
{
    Q_OBJECT
public:
    /// @brief          creates new SEQ view
    /// @param parent   parent QObject
    SEQView(QObject* parent = nullptr);
    /// @brief          gets scene from view
    /// @return         scene
    SEQScene* getScene() { return &scene; }
public slots:
    /// @brief          triggers scene update
    void update() { scene.update(); };
private:
    SEQScene scene;

};
