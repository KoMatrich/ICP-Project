/**
 * @file Service.h
 *
 * @brief Contains declaration of service
 * Used for easy creation and modification of services
 *
 * @author Martin Kocich
 * Contact: xkocic02@stud.fit.vutbr.cz
 *
 */

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
    Service(const Service&) {};
    /// @brief  prevents overwrite by assignment
    /// @param  self
    /// @return self
    Service& operator=(const Service&) {};
    /// @brief  prevents unwanted destruction
    ~Service() {};
};
