/**
 * @file type_cast.hpp
 * @brief
 */

#pragma once
#ifndef CPP_PIPE_OPERATOR_HELPER_TYPE_CAST_HPP
#define CPP_PIPE_OPERATOR_HELPER_TYPE_CAST_HPP

#include "pipe_operator_helper/detail/config.hpp"
#include "pipe_operator_helper/detail/bit_cast_impl.hpp"

namespace pipe_operator_helper {

namespace detail {

template<typename ToType>
struct static_cast_to_tag {
    using to_type = ToType;
};

template<typename ToType>
struct dynamic_cast_to_tag {
    using to_type = ToType;
};

template<typename ToType>
struct const_cast_to_tag {
    using to_type = ToType;
};

template<typename ToType>
struct reinterpret_cast_to_tag {
    using to_type = ToType;
};

template<typename ToType>
struct bit_cast_to_tag {
    using to_type = ToType;
};

}  // namespace detail

PIPE_OPERATOR_HELPER_MODULE_EXPORT_BEGIN

template<typename ToType>
constexpr detail::static_cast_to_tag<ToType> static_cast_to() noexcept
{
    return detail::static_cast_to_tag<ToType>{};
}

template<typename ToType>
constexpr detail::static_cast_to_tag<ToType> cast_to() noexcept
{
    return detail::static_cast_to_tag<ToType>{};
}

template<typename ToType>
detail::dynamic_cast_to_tag<ToType> dynamic_cast_to() noexcept
{
    return detail::dynamic_cast_to_tag<ToType>{};
}

template<typename ToType>
detail::const_cast_to_tag<ToType> const_cast_to() noexcept
{
    return detail::const_cast_to_tag<ToType>{};
}

template<typename ToType>
detail::reinterpret_cast_to_tag<ToType> reinterpret_cast_to() noexcept
{
    return detail::reinterpret_cast_to_tag<ToType>{};
}

template<typename ToType>
PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR detail::bit_cast_to_tag<ToType> bit_cast_to() noexcept
{
    return detail::bit_cast_to_tag<ToType>{};
}

inline namespace operators {
inline namespace type_cast_operators {

template<typename FromType, typename ToType>
constexpr ToType operator>(const FromType& from, const detail::static_cast_to_tag<ToType>&) noexcept
{
    return static_cast<ToType>(from);
}

template<typename FromType, typename ToType>
ToType operator>(const FromType& from, const detail::dynamic_cast_to_tag<ToType>&) noexcept
{
    return dynamic_cast<ToType>(from);
}

template<typename FromType, typename ToType>
ToType operator>(const FromType& from, const detail::const_cast_to_tag<ToType>&) noexcept
{
    return const_cast<ToType>(from);
}

template<typename FromType, typename ToType>
ToType operator>(const FromType& from, const detail::reinterpret_cast_to_tag<ToType>&) noexcept
{
    return reinterpret_cast<ToType>(from);
}

template<typename FromType, typename ToType>
PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR ToType operator>(const FromType& from,
                                                      const detail::bit_cast_to_tag<ToType>&) noexcept
{
    return detail::bit_cast_impl<ToType>(from);
}

}  // namespace type_cast_operators
}  // namespace operators

PIPE_OPERATOR_HELPER_MODULE_EXPORT_END

}  // namespace pipe_operator_helper

#endif  // !CPP_PIPE_OPERATOR_HELPER_TYPE_CAST_HPP
