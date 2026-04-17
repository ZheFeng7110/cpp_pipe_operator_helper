/**
 * @file 2args.cpp
 * @brief Tests for functions with 2 arguments
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
#endif

namespace {

namespace pipe_ = pipe_operator_helper;

// Example function with 2 arguments
constexpr int add(const int a, const int b) noexcept
{
    return a + b;
}

constexpr auto add(const int b) noexcept
{
    return pipe_::pipe_tag{[=](const int a) noexcept { return add(a, b); }};
}

}  // namespace

TEST(Pipe2ArgsTest, TypicalAdd)
{
    static_assert(2 == add(1, 1));
    static_assert(2 == (1 | add(1)));

    static_assert(114515 == add(add(114514, 1), 0));
    static_assert(114515 == (114514 | add(1) | add(0)));

    EXPECT_TRUE(noexcept(add(1, 1)));
    EXPECT_TRUE(noexcept(1 | add(1)));

    EXPECT_EQ(2, add(1, 1));
    EXPECT_EQ(2, 1 | add(1));

    EXPECT_EQ(114515, add(add(114514, 1), 0));
    EXPECT_EQ(114515, 114514 | add(1) | add(0));
}

namespace {

// Example function used exception
constexpr int divide(const int a, const int b)
{
    if (b == 0) {
        throw std::logic_error("divide by zero");
    }
    return a / b;
}

constexpr auto divide(const int b)
{
    return pipe_::pipe_tag{[=](const int a) { return divide(a, b); }};
}

}  // namespace

TEST(Pipe2ArgsTest, ExceptionTest)
{
    constexpr std::equal_to<int> eq;

    static_assert(requires { divide(1, 0); });
    static_assert(requires { 1 | divide(0); });

    static_assert(eq(2, divide(4, 2)));
    static_assert(eq(2, 4 | divide(2)));

    static_assert(eq(-1, divide(divide(4, 2), -2)));
    static_assert(eq(-1, 4 | divide(2) | divide(-2)));

    EXPECT_FALSE(noexcept(divide(1, 1)));
    EXPECT_FALSE(noexcept(1 | divide(1)));

    EXPECT_EQ(2, divide(4, 2));
    EXPECT_EQ(2, 4 | divide(2));

    EXPECT_THROW(divide(2, 0), std::logic_error);
    EXPECT_THROW(2 | divide(0), std::logic_error);

    EXPECT_EQ(-1, divide(divide(4, 2), -2));
    EXPECT_EQ(-1, 4 | divide(2) | divide(-2));

    EXPECT_THROW(divide(divide(-4, 2), 0), std::logic_error);
    EXPECT_THROW(-4 | divide(2) | divide(0), std::logic_error);
}

namespace {
namespace template_test {

// Example template with 2 arguments
template<typename T, typename U>
constexpr auto add(const T a, const U b) noexcept -> decltype(a + b)
{
    return a + b;
}

template<typename T, typename U>
constexpr auto add(const U b) noexcept
{
    return pipe_::pipe_tag{[=](const T a) noexcept { return add<T, U>(a, b); }};
}

}  // namespace template_test
}  // namespace

TEST(Pipe2ArgsTest, TemplateAdd)
{
    constexpr std::equal_to<double> eq;

    static_assert(2 == template_test::add(1, 1));
    static_assert(2 == (1 | template_test::add<int>(1)));

    static_assert(114515 == template_test::add(template_test::add(114514, 1), 0));
    static_assert(114515 == (114514 | template_test::add<int>(1) | template_test::add<int>(0)));

    static_assert(eq(2.0, template_test::add(1.0, 1)));
    static_assert(eq(2.0, 1.0 | template_test::add<double>(1)));

    EXPECT_TRUE(noexcept(template_test::add(1, 1)));
    EXPECT_TRUE(noexcept(1 | template_test::add<int>(1)));
    EXPECT_EQ(2, template_test::add(1, 1));
    EXPECT_EQ(2, 1 | template_test::add<int>(1));

    EXPECT_TRUE(noexcept(template_test::add(template_test::add(114514, 1), 0)));
    EXPECT_TRUE(noexcept(114514 | template_test::add<int>(1) | template_test::add<int>(0)));
    EXPECT_EQ(114515, template_test::add(template_test::add(114514, 1), 0));
    EXPECT_EQ(114515, 114514 | template_test::add<int>(1) | template_test::add<int>(0));

    EXPECT_TRUE(noexcept(template_test::add(1.0, 1)));
    EXPECT_TRUE(noexcept(1.0 | template_test::add<double>(1)));
    EXPECT_EQ(2.0, template_test::add(1.0, 1));
    EXPECT_EQ(2.0, 1.0 | template_test::add<double>(1));
}

#if (defined(TEST_IS_CPP20_OR_HIGHER) && !defined(_MSC_VER))

// consteval test
namespace {

consteval int bit_or(const int a, const int b) noexcept
{
    return a | b;
}

consteval auto bit_or(const int b) noexcept
{
    return pipe_::pipe_tag{[=](const int a) noexcept { return bit_or(a, b); }};
}

namespace test {
constexpr std::equal_to<int> eq;
}

static_assert(noexcept(bit_or(0b01, 0b10)));
static_assert(noexcept(0b01 | bit_or(0b10)));

static_assert(test::eq(0b11, bit_or(0b01, 0b10)));
static_assert(test::eq(0b11, 0b01 | bit_or(0b10)));

}  // namespace

#endif

// tests for reference
namespace {

constexpr void swap(int& a, int& b) noexcept
{
    const auto tmp = a;
    a = b;
    b = tmp;
}

constexpr auto swap(int& b) noexcept
{
    return pipe_::pipe_tag{[&](int& a) noexcept { ::swap(a, b); }};
}

}  // namespace

TEST(Pipe2ArgsTest, ReferenceTest)
{
    int a = 1, b = -1;

    swap(a, b);
    EXPECT_EQ(-1, a);
    EXPECT_EQ(1, b);

    a | swap(b);
    EXPECT_EQ(1, a);
    EXPECT_EQ(-1, b);
}

// callable object
namespace {

constexpr struct Multiply {
    constexpr int operator()(const int a, const int b) const noexcept
    {
        return a * b;
    }

    constexpr auto operator()(const int b) const noexcept
    {
        return pipe_::pipe_tag{[this, b](const int a) noexcept { return this->operator()(a, b); }};
    }
} multiply;

}  // namespace

TEST(Pipe2ArgsTest, CallableObjectTest)
{
    static_assert(6 == multiply(2, 3));
    static_assert(6 == (2 | multiply(3)));

    EXPECT_TRUE(noexcept(multiply(2, 3)));
    EXPECT_TRUE(noexcept(2 | multiply(3)));

    EXPECT_EQ(6, multiply(2, 3));
    EXPECT_EQ(6, 2 | multiply(3));
}

// NOLINTEND(*-use-transparent-functors)
