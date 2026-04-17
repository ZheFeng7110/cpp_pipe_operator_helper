/**
 * @file xmake_test.cpp
 * @brief
 */

#ifndef TEST_USE_MODULE
#include "pipe_operator_helper.hpp"

#include <iostream>
#else
#include <utility>
#include <iostream>

import pipe_operator_helper;
#endif

namespace pipe_ = pipe_operator_helper;

static int add(const int a, const int b) noexcept
{
    return a + b;
}

static auto add(const int b) noexcept
{
    return pipe_::pipe_tag{[=](const int a) noexcept { return add(a, b); }};
}

#define ASSERT(cond)         \
    do {                     \
        if (!(cond)) {       \
            return __LINE__; \
        }                    \
    } while (false)

auto main() -> int
{
    std::cout << "    Test begin..." << std::endl;

    ASSERT(2 == add(1, 1));
    ASSERT(2 == (1 | add(1)));

    std::cout << "    All test(s) passed." << std::endl;

    return 0;
}
