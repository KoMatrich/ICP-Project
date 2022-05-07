/**
 * @file History.h
 *
 * @brief Contains declaration of service that provides undo action
 *
 * @author Vitezslav Kriz
 * Contact xkrizv03@stud.fit.vutbr.cz
 *
 */

#pragma once

#include "Services/Highlight.h"

const int MAX_HIST_SIZE = 60;

/// @brief code editor service
class HistoryService : public Service<HistoryService>
{
private:
    /// @brief endpoint
    QTextEdit* code = nullptr;
public:
    /// @brief              sets global instance endpoint
    /// @param c            endpoint
    static void setEndpoint(QTextEdit* c);
    /// @brief              pushes new code snapshot to vector
    static void takeHistorySnapshot();
    /// @brief              pops code from vector to code editor
    static void restoreHistorySnapshot();

private:
    /// @brief              non static endpoint set
    /// @param c            endpoint
    void setCodeWindow(QTextEdit* c);
    /// @brief              Snapshot storage for ctrl+Z
    std::vector<QString> history;
};
