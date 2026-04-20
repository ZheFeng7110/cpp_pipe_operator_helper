cmake_minimum_required(VERSION 3.28...4.0)

project(cpp_pipe_operator_helper_test CXX)

message(STATUS "Configuring ${PROJECT_NAME}")
list(APPEND CMAKE_MESSAGE_INDENT "    ")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CPP_PIPE_OPERATOR_HELPER_TEST_CPP_STANDARD 11 CACHE STRING
    "Set the C++ standard for testing."
)
set_property(CACHE CPP_PIPE_OPERATOR_HELPER_TEST_CPP_STANDARD
    PROPERTY STRINGS "14;17;20;23"
)

if ((${CPP_PIPE_OPERATOR_HELPER_TEST_CPP_STANDARD} LESS 20) AND
(CPP_PIPE_OPERATOR_HELPER_USE_MODULES))
    message(FATAL_ERROR "Use modules requires C++20 or higher!")
endif ()

set(CMAKE_CXX_STANDARD "${CPP_PIPE_OPERATOR_HELPER_TEST_CPP_STANDARD}"
    CACHE INTERNAL "" FORCE
)

include("get_cpm.cmake")

block (SCOPE_FOR POLICIES)
    cmake_policy(VERSION 3.28...4.0.1)

    CPMAddPackage(
        NAME googletest
        GITHUB_REPOSITORY google/googletest
        GIT_TAG "v1.17.0"
        VERSION 1.17.0
        OPTIONS "BUILD_GMOCK OFF" "INSTALL_GTEST OFF"
    )

    if (NOT googletest_ADDED)
        message(FATAL_ERROR "Failed to add googletest")
    endif ()
endblock ()

if (MSVC)
    add_compile_options(/W4 /Zc:__cplusplus)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

set(testProjectName ${PROJECT_NAME})
set(cppStandard "${CPP_PIPE_OPERATOR_HELPER_TEST_CPP_STANDARD}")

add_executable(${testProjectName})

file(GLOB_RECURSE pipeTestSources "pipe_operators_tests/*.cpp")
target_sources(${testProjectName}
    PRIVATE test_main.cpp type_cast_test.cpp ${pipeTestSources}
    PRIVATE FILE_SET headers TYPE HEADERS FILES gtest_pch.hpp
)

message(DEBUG "Binary dir: ${CMAKE_CURRENT_BINARY_DIR}")

if ((${cppStandard} GREATER_EQUAL 20) AND (CPP_PIPE_OPERATOR_HELPER_USE_MODULES))
    set(CMAKE_CXX_SCAN_FOR_MODULES ON CACHE INTERNAL "" FORCE)
    target_compile_definitions(${testProjectName} PRIVATE TEST_USE_MODULE)
endif ()

target_link_libraries(${testProjectName} PRIVATE
    cpp_pipe_operator_helper
    GTest::gtest
)

if (NOT MSVC AND (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
    find_package(Threads REQUIRED)
    target_link_libraries(${testProjectName} PRIVATE Threads::Threads)
endif ()

target_precompile_headers(${testProjectName} PRIVATE gtest_pch.hpp)

add_test(
    NAME ${testProjectName}
    COMMAND ${testProjectName}
)
