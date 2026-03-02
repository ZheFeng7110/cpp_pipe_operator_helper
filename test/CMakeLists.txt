cmake_minimum_required(VERSION 3.28...4.0)

project(cpp_pipe_operator_helper_test CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CPP_PIPE_OPERATOR_HELPER_TEST_CPP_STANDARD 11 CACHE STRING
    "Set the C++ standard for testing."
)
set_property(CACHE CPP_PIPE_OPERATOR_HELPER_TEST_CPP_STANDARD
    PROPERTY STRINGS "11;14;17;20;23"
)

if ((${cppStandard} LESS 20) AND (CPP_PIPE_OPERATOR_HELPER_USE_MODULES))
    message(FATAL_ERROR "Use modules requires C++20 or higher!")
endif ()

set(CMAKE_CXX_STANDARD "${CPP_PIPE_OPERATOR_HELPER_TEST_CPP_STANDARD}"
    CACHE INTERNAL "" FORCE
)

include("find_gtest.cmake")

if (MSVC)
    add_compile_options(/W4 /Zc:__cplusplus)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif ()

function (declareTest testName cppStandard)
    set(testProjectName "${PROJECT_NAME}_${testName}")

    add_executable(${testProjectName} type_cast_test.cpp pipe_operator_test.cpp)

    message(DEBUG "[${PROJECT_NAME}]: Binary dir: ${CMAKE_CURRENT_BINARY_DIR}")

    if ((${cppStandard} GREATER_EQUAL 20) AND (CPP_PIPE_OPERATOR_HELPER_USE_MODULES))
        set(CMAKE_CXX_SCAN_FOR_MODULES ON CACHE INTERNAL "" FORCE)
        target_compile_definitions(${testProjectName} PRIVATE TEST_USE_MODULE)
    endif ()

    target_link_libraries(${testProjectName} PRIVATE cpp_pipe_operator_helper
        GTest::gtest GTest::gtest_main
    )

    if (NOT MSVC AND (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang"))
        find_package(Threads REQUIRED)
        target_link_libraries(${testProjectName} PRIVATE Threads::Threads)
    endif ()

    target_precompile_headers(${testProjectName} PRIVATE gtest_pch.hpp)

    add_test(NAME ${testProjectName}
        COMMAND ${testProjectName}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endfunction ()

declareTest("all" "${CPP_PIPE_OPERATOR_HELPER_TEST_CPP_STANDARD}")
