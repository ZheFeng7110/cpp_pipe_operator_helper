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
    constexpr auto tuple_size = std::tuple_size<typename std::remove_reference<Tuple>::type>::value;
    return invoke_with_1arg_1tuple_impl(std::forward<Func>(func),
                                        std::forward<First>(first),
                                        std::forward<Tuple>(tuple),
                                        std::make_index_sequence<tuple_size>{});
}

}  // namespace detail

PIPE_OPERATOR_HELPER_MODULE_EXPORT_BEGIN

template<typename R, typename Func, typename FirstArg, typename... RemainArgs>
class pipe_tag
{
public:
    using return_type = R;
    using function_type = Func;

protected:
    function_type& func_;
    std::tuple<RemainArgs...> remain_args_;

public:
    explicit constexpr pipe_tag(Func&& func, RemainArgs&&... remain_args) noexcept
        : func_{std::forward<Func>(func)}, remain_args_{std::forward<RemainArgs>(remain_args)...}
    {
    }

    friend return_type operator|(FirstArg&& first_arg, const pipe_tag& pipe) noexcept(
        detail::is_invoke_with_1arg_1tuple_noexcept_v<Func, FirstArg, decltype(pipe.remain_args_)>)
    {
        return detail::invoke_with_1arg_1tuple(std::forward<Func>(pipe.func_),
                                               std::forward<FirstArg>(first_arg),
                                               pipe.remain_args_);
    }
};

#if (__cplusplus >= 202002L)

template<typename R, typename Func, typename FirstArg, typename... RemainArgs>
class constexpr_pipe_tag : public pipe_tag<R, Func, FirstArg, RemainArgs...>
{
private:
    using super_type_ = pipe_tag<R, Func, FirstArg, RemainArgs...>;

public:
    using return_type = typename super_type_::return_type;
    using function_type = typename super_type_::function_type;

    using super_type_::super_type_;

    friend constexpr return_type operator|(FirstArg&& first_arg, const constexpr_pipe_tag& pipe) noexcept(
        detail::is_invoke_with_1arg_1tuple_noexcept_v<Func, FirstArg, decltype(pipe.remain_args_)>)
    {
        return detail::invoke_with_1arg_1tuple(std::forward<Func>(pipe.func_),
                                               std::forward<FirstArg>(first_arg),
                                               pipe.remain_args_);
    }
};

template<typename R, typename Func, typename FirstArg, typename... RemainArgs>
class consteval_pipe_tag : public pipe_tag<R, Func, FirstArg, RemainArgs...>
{
private:
    using super_type_ = pipe_tag<R, Func, FirstArg, RemainArgs...>;

public:
    using return_type = typename super_type_::return_type;
    using function_type = typename super_type_::function_type;

    using super_type_::super_type_;

    friend consteval return_type operator|(FirstArg&& first_arg, const consteval_pipe_tag& pipe) noexcept(
        detail::is_invoke_with_1arg_1tuple_noexcept_v<Func, FirstArg, decltype(pipe.remain_args_)>)
    {
        return detail::invoke_with_1arg_1tuple(std::forward<Func>(pipe.func_),
                                               std::forward<FirstArg>(first_arg),
                                               pipe.remain_args_);
    }
};

#endif

PIPE_OPERATOR_HELPER_MODULE_EXPORT_END

}  // namespace pipe_operator_helper

#endif  // !CPP_PIPE_OPERATOR_HELPER_PIPE_TAG_HPP
