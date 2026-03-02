/**
 * @file dll_dummy_export.cpp
 * @brief
 */

module;

module pipe_operator_helper;

namespace pipe_operator_helper {
namespace detail {

__declspec(dllexport) void dll_dummy_export() noexcept {}

}  // namespace detail
}  // namespace pipe_operator_helper
