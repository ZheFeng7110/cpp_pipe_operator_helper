/**
 * @file pipe_tag.hpp
 * @brief
 */

// ReSharper disable CppRedundantInlineSpecifier

#pragma once
#ifndef CPP_PIPE_OPERATOR_HELPER_PIPE_TAG_HPP
#define CPP_PIPE_OPERATOR_HELPER_PIPE_TAG_HPP

#include "pipe_operator_helper/detail/config.hpp"

#ifndef PIPE_OPERATOR_HELPER_MODULE_INTERFACE_UNIT
#include <utility>
#include <tuple>
#include <type_traits>
#include <functional>
#endif

namespace pipe_operator_helper {
namespace detail {

template<typename Func, typename First, typename... Remains>
struct is_invoke_with_1arg_1tuple_noexcept : std::false_type {
};

template<typename Func, typename First, typename... Remains>
struct is_invoke_with_1arg_1tuple_noexcept<Func, First, std::tuple<Remains...>>
    : std::is_nothrow_invocable<Func, First, Remains...> {
};

template<typename Func, typename First, typename... Remains>
struct is_invoke_with_1arg_1tuple_noexcept<Func, First, std::tuple<Remains...>&>
    : std::is_nothrow_invocable<Func, First, Remains&...> {
};

template<typename Func, typename First, typename... Remains>
struct is_invoke_with_1arg_1tuple_noexcept<Func, First, const std::tuple<Remains...>>
    : std::is_nothrow_invocable<Func, First, const Remains...> {
};

template<typename Func, typename First, typename... Remains>
struct is_invoke_with_1arg_1tuple_noexcept<Func, First, const std::tuple<Remains...>&>
    : std::is_nothrow_invocable<Func, First, const Remains&...> {
};

template<typename Func, typename First, typename... Remains>
struct is_invoke_with_1arg_1tuple_noexcept<Func, First, std::tuple<Remains...>&&>
    : std::is_nothrow_invocable<Func, First, Remains&&...> {
};

template<typename Func, typename First, typename... Remains>
struct is_invoke_with_1arg_1tuple_noexcept<Func, First, const std::tuple<Remains...>&&>
    : std::is_nothrow_invocable<Func, First, const Remains&&...> {
};

template<typename Func, typename First, typename... Remains>
constexpr bool is_invoke_with_1arg_1tuple_noexcept_v =
    is_invoke_with_1arg_1tuple_noexcept<Func, First, Remains...>::value;

template<typename Func, typename First, typename Tuple, std::size_t... Index>
inline PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR decltype(auto) invoke_with_1arg_1tuple_impl(Func&& func,
                                                                                        First&& first,
                                                                                        Tuple&& tuple,
                                                                                        std::index_sequence<Index...>)
{
    return std::invoke(std::forward<Func>(func),
                       std::forward<First>(first),
                       std::get<Index>(std::forward<Tuple>(tuple))...);
}

template<typename Func, typename First, typename Tuple>
inline PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR decltype(auto) invoke_with_1arg_1tuple(Func&& func,
                                                                                   First&& first,
                                                                                   Tuple&& tuple)
{
    constexpr auto tuple_size = std::tuple_size_v<std::remove_reference_t<Tuple>>;
    return invoke_with_1arg_1tuple_impl(std::forward<Func>(func),
                                        std::forward<First>(first),
                                        std::forward<Tuple>(tuple),
                                        std::make_index_sequence<tuple_size>{});
}

}  // namespace detail

PIPE_OPERATOR_HELPER_MODULE_EXPORT_BEGIN

template<typename Func, typename... SecondAndSubsequentArgs>
class pipe_tag
{
public:
    using function_type = Func;
    using second_and_subsequent_args_type = std::tuple<SecondAndSubsequentArgs...>;

protected:
    function_type func_;
    second_and_subsequent_args_type remain_args_;

public:
    // ReSharper disable once CppNonExplicitConvertingConstructor
    constexpr pipe_tag(Func&& func, SecondAndSubsequentArgs&&... second_and_subsequent_args) noexcept
        : func_{std::forward<Func>(func)},
          remain_args_{std::forward<SecondAndSubsequentArgs>(second_and_subsequent_args)...}
    {
    }

    constexpr const second_and_subsequent_args_type& get_remain_args() const& noexcept
    {
        return remain_args_;
    }
    constexpr second_and_subsequent_args_type& get_remain_args() & noexcept
    {
        return remain_args_;
    }
    constexpr second_and_subsequent_args_type&& get_remain_args() && noexcept
    {
        return std::move(remain_args_);
    }

    constexpr const function_type& get_function() const& noexcept
    {
        return func_;
    }
    constexpr function_type& get_function() & noexcept
    {
        return func_;
    }
    constexpr function_type&& get_function() && noexcept
    {
        return std::move(func_);
    }
};

template<typename FirstArg, typename Func, typename... SecondAndSubsequentArgs>
inline decltype(auto)
operator|(FirstArg&& first_arg, const pipe_tag<Func, SecondAndSubsequentArgs...>& pipe_tag_value) noexcept(
    detail::is_invoke_with_1arg_1tuple_noexcept_v<Func, FirstArg, decltype(pipe_tag_value.get_remain_args())>)
{
    return detail::invoke_with_1arg_1tuple(std::forward<Func>(pipe_tag_value.get_function()),
                                           std::forward<FirstArg>(first_arg),
                                           pipe_tag_value.get_remain_args());
}
template<typename FirstArg, typename Func, typename... SecondAndSubsequentArgs>
inline decltype(auto)
operator|(FirstArg&& first_arg, pipe_tag<Func, SecondAndSubsequentArgs...>&& pipe_tag_value) noexcept(
    detail::
        is_invoke_with_1arg_1tuple_noexcept_v<Func, FirstArg, decltype(std::move(pipe_tag_value).get_remain_args())>)
{
    return detail::invoke_with_1arg_1tuple(std::forward<Func>(std::move(pipe_tag_value).get_function()),
                                           std::forward<FirstArg>(first_arg),
                                           std::move(pipe_tag_value).get_remain_args());
}

#if (__cplusplus >= 202002L)

// For constexpr
template<typename Func, typename... SecondAndSubsequentArgs>
class constexpr_pipe_tag : public pipe_tag<Func, SecondAndSubsequentArgs...>
{
private:
    using super_ = pipe_tag<Func, SecondAndSubsequentArgs...>;

public:
    using super_::super_;
};

template<typename FirstArg, typename Func, typename... SecondAndSubsequentArgs>
inline constexpr decltype(auto)
operator|(FirstArg&& first_arg, const constexpr_pipe_tag<Func, SecondAndSubsequentArgs...>& pipe_tag_value) noexcept(
    detail::is_invoke_with_1arg_1tuple_noexcept_v<Func, FirstArg, decltype(pipe_tag_value.get_remain_args())>)
{
    return detail::invoke_with_1arg_1tuple(std::forward<Func>(pipe_tag_value.get_function()),
                                           std::forward<FirstArg>(first_arg),
                                           pipe_tag_value.get_remain_args());
}
template<typename FirstArg, typename Func, typename... SecondAndSubsequentArgs>
inline constexpr decltype(auto)
operator|(FirstArg&& first_arg, constexpr_pipe_tag<Func, SecondAndSubsequentArgs...>&& pipe_tag_value) noexcept(
    detail::
        is_invoke_with_1arg_1tuple_noexcept_v<Func, FirstArg, decltype(std::move(pipe_tag_value).get_remain_args())>)
{
    return detail::invoke_with_1arg_1tuple(std::forward<Func>(std::move(pipe_tag_value).get_function()),
                                           std::forward<FirstArg>(first_arg),
                                           std::move(pipe_tag_value).get_remain_args());
}

// For consteval
template<typename Func, typename... SecondAndSubsequentArgs>
class consteval_pipe_tag : public pipe_tag<Func, SecondAndSubsequentArgs...>
{
private:
    using super_ = pipe_tag<Func, SecondAndSubsequentArgs...>;

public:
    using super_::super_;
};

template<typename FirstArg, typename Func, typename... SecondAndSubsequentArgs>
inline consteval decltype(auto)
operator|(FirstArg&& first_arg, const consteval_pipe_tag<Func, SecondAndSubsequentArgs...>& pipe_tag_value) noexcept(
    detail::is_invoke_with_1arg_1tuple_noexcept_v<Func, FirstArg, decltype(pipe_tag_value.get_remain_args())>)
{
    return detail::invoke_with_1arg_1tuple(std::forward<Func>(pipe_tag_value.get_function()),
                                           std::forward<FirstArg>(first_arg),
                                           pipe_tag_value.get_remain_args());
}
template<typename FirstArg, typename Func, typename... SecondAndSubsequentArgs>
inline consteval decltype(auto)
operator|(FirstArg&& first_arg, consteval_pipe_tag<Func, SecondAndSubsequentArgs...>&& pipe_tag_value) noexcept(
    detail::
        is_invoke_with_1arg_1tuple_noexcept_v<Func, FirstArg, decltype(std::move(pipe_tag_value).get_remain_args())>)
{
    return detail::invoke_with_1arg_1tuple(std::forward<Func>(std::move(pipe_tag_value).get_function()),
                                           std::forward<FirstArg>(first_arg),
                                           std::move(pipe_tag_value).get_remain_args());
}

#endif  // (__cplusplus >= 202002L)

template<typename T>
constexpr decltype(auto) forward_arg(std::remove_reference_t<T>& value) noexcept
{
    return std::forward<T>(value);
}
template<typename T>
constexpr decltype(auto) forward_arg(std::remove_reference_t<T>&& value) noexcept
{
    return std::forward<T>(value);
}

PIPE_OPERATOR_HELPER_MODULE_EXPORT_END

}  // namespace pipe_operator_helper

#endif  // !CPP_PIPE_OPERATOR_HELPER_PIPE_TAG_HPP
