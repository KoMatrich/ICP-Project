/**
 * @file Highlight.h
 *
 * @brief Contains declaration of service for disabling text analysis
 * Used for preventing infinite recursion and to boost performance
 *
 * @author Martin Kocich
 * Contact: xkocic02@stud.fit.vutbr.cz
 *
 */

#pragma once

#include "Services/Service.h"

 /// @brief highlight service
 /// used to disable enable highlighter
class HighlightService : public Service<HighlightService>
{
private:
    /// @brief  state
    bool enabled = true;
public:
    /// @brief      set current state
    /// @param e    state
    static void setEnabled(bool e);
    /// @brief      get current state
    /// @return     current state
    static bool getEnabled();
};
