/**
 * @file SEQView.h
 *
 * @brief Contains declaration of view for SEQ
 *
 * @author Martin Kocich
 * Contact: xkocic02@stud.fit.vutbr.cz
 *
 */

#pragma once

#include <QWidget>
#include "Semantics.h"

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
    /// @brief Adds new column (1 column per member)
    /// @param member member present in Sequence
    /// @param pos XY point for new column
    /// @param off offset
    /// @param height height of column calculated from action vector size
    void addColumn(SEQMember& member, QPoint pos, QPoint& off, const int& height);
    /// @brief Adds new arrow (1 arrow per Action)
    /// @param action action present in Sequence
    /// @param timeIndex time of action (used for calculating its y value)
    /// @param pos pos XY point for new arrow
    /// @param off offset
    void addArrow(SEQAction& action, uint& timeIndex, std::vector<QPoint> pos, std::vector<QPoint> off);
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
    /// @brief scene of this view
    SEQScene scene;

};
