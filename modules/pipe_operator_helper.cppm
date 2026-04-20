/**
 * @file pipe_operator_helper.cppm
 * @brief Lib pipe_operator_helper module interface unit
 */

module;

#if (__cplusplus >= 202002L)  // C++20
#include <bit>
#else
#include <cstring>
#endif

#include <utility>
#include <type_traits>
#include <functional>

#define PIPE_OPERATOR_HELPER_MODULE_INTERFACE_UNIT

export module pipe_operator_helper;

#ifdef __clang__
#pragma clang diagnostic ignored "-Winclude-angled-in-module-purview"
#endif

#include "pipe_operator_helper.hpp"
