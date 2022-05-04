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
