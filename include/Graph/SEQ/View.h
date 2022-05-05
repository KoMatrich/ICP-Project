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
    void addColumn(SEQMember member, QPointF& offsetPos, const int& height);
    void addArrow(SEQMember action, QPointF& offsetPos);
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
