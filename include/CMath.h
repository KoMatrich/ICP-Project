#pragma once

/// @brief          returns sign
/// @tparam T       comparable type
/// @param val      value
/// @return         sign or value or 0
template <typename T> int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}
