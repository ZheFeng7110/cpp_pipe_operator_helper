cmake_minimum_required(VERSION 3.28...4.0)

file(READ "version.txt" cppPipeOperatorHelperVersion)

project(cpp_pipe_operator_helper
    LANGUAGES CXX
    VERSION "${cppPipeOperatorHelperVersion}"
)

message(STATUS "[${PROJECT_NAME}]: Version: ${PROJECT_VERSION}")

option(CPP_PIPE_OPERATOR_HELPER_USE_MODULES
    "Use C++20 modules for cpp_pipe_operator_helper"
    OFF
)
option(CPP_PIPE_OPERATOR_HELPER_BUILD_SHARED
    "Build cpp_pipe_operator_helper as a shared library if \
`CPP_PIPE_OPERATOR_HELPER_USE_MODULES` ON"
    OFF
)
option(CPP_PIPE_OPERATOR_HELPER_BUILD_TESTS
    "Build tests for cpp_pipe_operator_helper"
    OFF
)

if (CPP_PIPE_OPERATOR_HELPER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()

if (CPP_PIPE_OPERATOR_HELPER_USE_MODULES)
    if (CPP_PIPE_OPERATOR_HELPER_BUILD_SHARED)
        add_library(${PROJECT_NAME} SHARED)
        message(STATUS "[${PROJECT_NAME}]: provides module with shared library.")
    else ()
        add_library(${PROJECT_NAME} STATIC)
        message(STATUS "[${PROJECT_NAME}]: provides module with static library.")
    endif ()

    target_include_directories(${PROJECT_NAME} PUBLIC include)
    target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET ${PROJECT_NAME}_module TYPE CXX_MODULES FILES
            pipe_operator_helper.cppm
    )
else ()
    if (CPP_PIPE_OPERATOR_HELPER_BUILD_SHARED)
        message(WARNING "[${PROJECT_NAME}]: `CPP_PIPE_OPERATOR_HELPER_BUILD_SHARED` ignored "
            "because `CPP_PIPE_OPERATOR_HELPER_USE_MODULES` was OFF. Set "
            "`CPP_PIPE_OPERATOR_HELPER_BUILD_SHARED` OFF to avoid this warning."
        )
    endif ()

    add_library(${PROJECT_NAME} INTERFACE)
    target_include_directories(${PROJECT_NAME} INTERFACE include)

    message(STATUS "[${PROJECT_NAME}]: provides headers.")
endif ()
