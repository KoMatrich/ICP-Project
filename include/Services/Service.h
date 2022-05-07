#pragma once

/// @brief "abstract" service class
/// global singleton allocated on stack
/// for fast access
/// @note bad practice to use this!
template<typename T>
class Service
{
public:
    /// @brief      gets global instance
    /// @return     this instance
    static T& getInstance()
    {
        static T instance{};// guaranteed to be destruction
        return instance;    // instantiated on first use
    }
protected:
    /// @brief prevents construction
    Service() {};
    /// @brief prevents construction by copying
    /// @param self
    Service(const Service&) final {};
    /// @brief  prevents overwrite by assignment
    /// @param  self
    /// @return self
    Service& operator=(const Service&) {};
    /// @brief  prevents unwanted destruction
    ~Service() {};
};
