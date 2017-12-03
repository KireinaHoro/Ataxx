#ifndef UTILS_H
#define UTILS_H

#include <type_traits>

template<typename Enum>
auto as_integer(Enum const value)
{
    return static_cast<typename std::underlying_type<Enum>::type>(value);
}

#endif // UTILS_H
