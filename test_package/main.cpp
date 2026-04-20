#include <iostream>

#ifdef TEST_USE_MODULE
import pipe_operator_helper;
#else
#include <pipe_operator_helper.hpp>
#endif

int add(int a, int b) noexcept
{
    return a + b;
}

auto add(int b) noexcept
{
    return pipe_operator_helper::pipe_tag{[&](int a) noexcept { return add(a, b); }};
}

int main()
{
    std::cout << "add(1, 1) == " << add(1, 1) << std::endl;
    std::cout << "(1 | add(a)) == " << (1 | add(1)) << std::endl;

    std::cout << "\n    Package test PASS!" << std::endl;
}
