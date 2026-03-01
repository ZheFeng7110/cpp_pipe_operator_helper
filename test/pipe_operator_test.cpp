/**
 * @file pipe_operator_test.cpp
 * @brief
 */

#ifndef TEST_USE_MODULE
#include "pipe_operator_helper.hpp"
#endif

#include "gtest_pch.hpp"

#include <stdexcept>
#include <functional>

#if (__cplusplus >= 202002L)
#define TEST_IS_CPP20_OR_HIGHER
#endif

#ifdef TEST_USE_MODULE
import pipe_operator_helper;

// While use module, we can assume that the C++ standard is C++20 or higher
#define PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR constexpr
#endif

namespace pipe = pipe_operator_helper;

// Example function with 2 arguments
static PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR int add(const int a, const int b) noexcept
{
    return a + b;
}

static PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR auto add(const int b) noexcept
#ifdef TEST_IS_CPP20_OR_HIGHER
    -> pipe::constexpr_pipe_tag<int, int (&)(int, int), const int, const int>
#else
    -> pipe::pipe_tag<int, int (&)(int, int), const int, const int>
#endif
{
    return
#ifdef TEST_IS_CPP20_OR_HIGHER
        pipe::constexpr_pipe_tag<int, int (&)(int, int), const int, const int>
#else
        pipe::pipe_tag<int, int (&)(int, int), const int, const int>
#endif
        {add, std::forward<const int>(b)};
}

TEST(PipeTest, Add)
{
#ifdef TEST_IS_CPP20_OR_HIGHER
    static_assert(2 == add(1, 1));
    static_assert(2 == (1 | add(1)));

    static_assert(114515 == add(add(114514, 1), 0));
    static_assert(114515 == (114514 | add(1) | add(0)));
#endif

    EXPECT_EQ(2, add(1, 1));
    EXPECT_EQ(2, 1 | add(1));

    EXPECT_EQ(114515, add(add(114514, 1), 0));
    EXPECT_EQ(114515, 114514 | add(1) | add(0));
}

// Example function used exception
static PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR int divide(const int a, const int b)
{
    if (b == 0) {
        throw std::logic_error("divide by zero");
    }
    return a / b;
}

static PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR auto divide(const int b)
#ifdef TEST_IS_CPP20_OR_HIGHER
    -> pipe::constexpr_pipe_tag<int, int (&)(int, int), const int, const int>
#else
    -> pipe::pipe_tag<int, int (&)(int, int), const int, const int>
#endif
{
    return
#ifdef TEST_IS_CPP20_OR_HIGHER
        pipe::constexpr_pipe_tag<int, int (&)(int, int), const int, const int>
#else
        pipe::pipe_tag<int, int (&)(int, int), const int, const int>
#endif
        {divide, std::forward<const int>(b)};
}

TEST(PipeTest, Divide)
{
#ifdef TEST_IS_CPP20_OR_HIGHER
    constexpr std::equal_to<int> eq;

    static_assert(eq(2, divide(4, 2)));
    static_assert(eq(2, 4 | divide(2)));

    static_assert(eq(-1, divide(divide(4, 2), -2)));
    static_assert(eq(-1, 4 | divide(2) | divide(-2)));

    static_assert(requires { divide(1, 0); });
    static_assert(requires { 1 | divide(0); });
#endif

    EXPECT_EQ(2, divide(4, 2));
    EXPECT_EQ(2, 4 | divide(2));

    EXPECT_THROW(divide(2, 0), std::logic_error);
    EXPECT_THROW(2 | divide(0), std::logic_error);

    EXPECT_EQ(-1, divide(divide(4, 2), -2));
    EXPECT_EQ(-1, 4 | divide(2) | divide(-2));

    EXPECT_THROW(divide(divide(-4, 2), 0), std::logic_error);
    EXPECT_THROW(-4 | divide(2) | divide(0), std::logic_error);
}

// Example function with 1 argument
static PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR bool logic_not(const bool b) noexcept
{
    return !b;
}

static PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR auto logic_not() noexcept
#ifdef TEST_IS_CPP20_OR_HIGHER
    -> pipe::constexpr_pipe_tag<bool, bool (&)(bool), const bool>
#else
    -> pipe::pipe_tag<bool, bool (&)(bool), const bool>
#endif
{
    return
#ifdef TEST_IS_CPP20_OR_HIGHER
        pipe::constexpr_pipe_tag<bool, bool (&)(bool), const bool>
#else
        pipe::pipe_tag<bool, bool (&)(bool), const bool>
#endif
        {::logic_not};
}

TEST(PipeTest, LogicNot)
{
#ifdef TEST_IS_CPP20_OR_HIGHER
    constexpr std::equal_to<int> eq;

    static_assert(eq(true, logic_not(false)));
    static_assert(eq(true, false | logic_not()));

    static_assert(eq(false, logic_not(logic_not(false))));
    static_assert(eq(false, false | logic_not() | logic_not()));
#endif

    EXPECT_EQ(true, logic_not(false));
    EXPECT_EQ(true, false | logic_not());

    EXPECT_EQ(false, logic_not(logic_not(false)));
    EXPECT_EQ(false, false | logic_not() | logic_not());
}

#ifdef TEST_IS_CPP20_OR_HIGHER

consteval int bit_or(const int a, const int b) noexcept
{
    return a | b;
}

consteval auto bit_or(const int b) noexcept -> pipe::consteval_pipe_tag<int, int (&)(int, int), const int, const int>
{
    return pipe::consteval_pipe_tag<int, int (&)(int, int), const int, const int>{bit_or, std::forward<const int>(b)};
}

namespace test {
constexpr std::equal_to<int> eq;
}

static_assert(test::eq(0b11, bit_or(0b01, 0b10)));
static_assert(test::eq(0b11, 0b01 | bit_or(0b10)));

#endif
