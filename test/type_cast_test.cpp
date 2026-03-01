/**
 * @file type_cast_test.cpp
 * @brief
 */

#ifndef TEST_USE_MODULE
#include "pipe_operator_helper.hpp"
#endif

#include "gtest_pch.hpp"

#include <cstdint>
#include <cstring>

#include <type_traits>

#ifdef TEST_USE_MODULE
import pipe_operator_helper;
#endif

namespace pipe = pipe_operator_helper;
using namespace pipe::type_cast_operators;

#define STATIC_CAST_TEST(value, to_type) \
    EXPECT_EQ(static_cast<to_type>(value), (value) > pipe::static_cast_to<to_type>())

TEST(StaticCast, BasicType)
{
    STATIC_CAST_TEST(123.0f, int);
    STATIC_CAST_TEST(114.514f, double);
    STATIC_CAST_TEST(114514, double);
}

TEST(StaticCast, Pointer)
{
    int value = 1919;
    int* ptr = &value;
    EXPECT_EQ(ptr, ptr > pipe::cast_to<int*>());
    EXPECT_EQ(value, *(ptr > pipe::cast_to<int*>()));

    auto vptr = &value > pipe::cast_to<void*>();
    EXPECT_EQ(value, *(vptr > pipe::cast_to<int*>()));

    EXPECT_EQ(value, *(ptr > pipe::cast_to<void*>() > pipe::cast_to<int*>()));
}

TEST(StaticCast, Class)
{
    struct Base {
        int a;
        float b;

        bool operator==(const Base& other) const
        {
            return a == other.a && b == other.b;
        }
    };

    struct Derived : Base {
        char c;
        short d;
    };

    Base base{114, 5.14f};
    Derived derived{{114, 5.14f}, 'a', 1919};
    EXPECT_EQ(base, derived > pipe::cast_to<Base>());

    Base& base_ref = base;
    Derived& derived_ref = derived;
    EXPECT_EQ(base_ref, derived_ref > pipe::cast_to<const Base&>());

    const Base& base_cref = base;
    const Derived& derived_cref = derived;
    EXPECT_EQ(base_cref, derived_cref > pipe::cast_to<const Base&>());

    Base* base_ptr = &base;
    Derived* derived_ptr = &derived;
    EXPECT_EQ(*base_ptr, *(derived_ptr > pipe::cast_to<Base*>()));
}

#undef STATIC_CAST_TEST

TEST(DynamicCast, Class)
{
    struct Base {
        virtual ~Base() = default;

        int a;
        short b;
        char c;

        Base(const int a_, const short b_, const char c_) : a(a_), b(b_), c(c_) {}

        [[nodiscard]] virtual int getA() const
        {
            return a;
        }

        virtual short func()
        {
            return 0;
        }

        bool operator==(const Base& other) const
        {
            return a == other.a && b == other.b && c == other.c;
        }
    };

    struct Derived : Base {
        int a_;
        char c;
        short d;

        Derived(const Base& base, const int a, const char c_, const short d_) : Base(base), a_(a), c(c_), d(d_) {}

        [[nodiscard]] int getA() const override
        {
            return a_;
        }

        short func() override
        {
            return 214;
        }
    };

    Base base{1, 2, 'c'};
    Derived derived{{1, 2, 'c'}, 114, 'a', 1919};

    auto derived_casted = &derived > pipe::dynamic_cast_to<Base*>();
    ASSERT_NE(derived_casted, nullptr);
    EXPECT_EQ(base, *derived_casted);

    ASSERT_EQ(nullptr, &base > pipe::dynamic_cast_to<Derived*>());
}

TEST(ConstCast, BasicType)
{
    int a = 114;
    const int* a_cptr = &a;
    EXPECT_EQ(a_cptr, a_cptr > pipe::const_cast_to<int*>());
    EXPECT_EQ(*a_cptr, *(a_cptr > pipe::const_cast_to<int*>()));
    EXPECT_EQ(a_cptr, a_cptr > pipe::const_cast_to<volatile int*>());
    EXPECT_EQ(*a_cptr, *(a_cptr > pipe::const_cast_to<volatile int*>()));

    const int& a_cref = a;
    EXPECT_EQ(a, a_cref);
    EXPECT_EQ(a_cref, a_cref > pipe::const_cast_to<int&>());
    EXPECT_EQ(a_cref, a_cref > pipe::const_cast_to<volatile int&>());

    short b = -1454;
    volatile short* b_vptr = &b;
    EXPECT_EQ(b_vptr, b_vptr > pipe::const_cast_to<short*>());
    EXPECT_EQ(*b_vptr, *(b_vptr > pipe::const_cast_to<short*>()));
    EXPECT_EQ(b_vptr, b_vptr > pipe::const_cast_to<const short*>());
    EXPECT_EQ(*b_vptr, *(b_vptr > pipe::const_cast_to<const short*>()));

    volatile short& b_vref = b;
    EXPECT_EQ(b, b_vref);
    EXPECT_EQ(b_vref, b_vref > pipe::const_cast_to<short&>());
    EXPECT_EQ(b_vref, b_vref > pipe::const_cast_to<const short&>());
}

template<typename T>
struct none_ptr_store_t {
    static_assert(std::is_void<T>::value, "Could not implements `ptr_store_t`");
    using type = T;
};

using ptr_store_t =
    std::conditional<sizeof(unsigned int) == sizeof(int*),
                     unsigned int,
                     std::conditional<sizeof(unsigned long) == sizeof(int*),
                                      unsigned long,
                                      std::conditional<sizeof(unsigned long long) == sizeof(int*),
                                                       unsigned long long,
                                                       none_ptr_store_t<void>::type>::type>::type>::type;

TEST(ReinterpretCast, BasicType)
{
    int a = 3;
    int* pa = &a;

    auto pa_value = pa > pipe::reinterpret_cast_to<ptr_store_t>();
    EXPECT_EQ(pa_value, reinterpret_cast<ptr_store_t>(pa));

    auto v_to_pa = pa_value > pipe::reinterpret_cast_to<int*>();
    EXPECT_EQ(v_to_pa, reinterpret_cast<int*>(pa_value));
    EXPECT_EQ(pa, v_to_pa);
    EXPECT_EQ(a, *v_to_pa);
}

TEST(BitCast, BasicType)
{
    int a = 3;
    int* pa = &a;

    auto pa_value = pa > pipe::bit_cast_to<ptr_store_t>();
    ptr_store_t expect_pa_value;
    std::memcpy(&expect_pa_value, &pa, sizeof(int*));
    EXPECT_EQ(pa_value, expect_pa_value);

    auto v_to_pa = pa_value > pipe::bit_cast_to<int*>();
    int* expect_v_to_pa;
    std::memcpy(&expect_v_to_pa, &pa_value, sizeof pa_value);
    EXPECT_EQ(v_to_pa, expect_v_to_pa);
    EXPECT_EQ(pa, v_to_pa);
    EXPECT_EQ(a, *v_to_pa);
}
