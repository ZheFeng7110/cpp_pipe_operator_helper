/**
 * @file 1arg.cpp
 * @brief Tests for functions with 1 argument
 */

// NOLINTBEGIN(*-use-transparent-functors)

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

namespace {

namespace pipe_ = pipe_operator_helper;

// Example function with 1 argument
PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR bool logic_not(const bool b) noexcept
{
    return !b;
}

PIPE_OPERATOR_HELPER_CPP20_CONSTEXPR auto logic_not() noexcept
{
    return pipe_::pipe_tag{[](const bool b) noexcept { return logic_not(b); }};
}

}  // namespace

TEST(Pipe1ArgTest, LogicNot)
{
#if (defined(TEST_IS_CPP20_OR_HIGHER) && !defined(_MSC_VER))
    constexpr std::equal_to<int> eq;

    static_assert(eq(true, logic_not(false)));
    static_assert(eq(true, false | logic_not()));

    static_assert(eq(false, logic_not(logic_not(false))));
    static_assert(eq(false, false | logic_not() | logic_not()));
#endif

    EXPECT_TRUE(noexcept(logic_not(false)));
    EXPECT_TRUE(noexcept(false | logic_not()));

    EXPECT_EQ(true, logic_not(false));
    EXPECT_EQ(true, false | logic_not());

    EXPECT_EQ(false, logic_not(logic_not(false)));
    EXPECT_EQ(false, false | logic_not() | logic_not());
}

namespace {

// Example function with reference
constexpr void add1(int& v) noexcept
{
    ++v;
}

constexpr auto add1() noexcept
{
    return pipe_::pipe_tag{[](int& v) noexcept { add1(v); }};
}

#if (defined(TEST_IS_CPP20_OR_HIGHER) && !defined(_MSC_VER))
consteval bool test_() noexcept
{
    int a = 0;

    add1(a);
    if (a != 1) {
        return false;
    }

    a | add1();
    if (a != 2) {
        return false;
    }

    return true;
}
static_assert(test_(), "");
#endif

}  // namespace

TEST(Pipe1ArgTest, Add1)
{
    int a = 0;

    EXPECT_TRUE(noexcept(add1(a)));

    add1(a);
    EXPECT_EQ(a, 1);

    a | add1();
    EXPECT_EQ(a, 2);
}

// NOLINTEND(*-use-transparent-functors)
