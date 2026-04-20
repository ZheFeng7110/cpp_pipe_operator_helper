cmake_minimum_required(VERSION 3.28...4.0)

file(READ "version.txt" cppPipeOperatorHelperVersion)
string(STRIP "${cppPipeOperatorHelperVersion}" cppPipeOperatorHelperVersion)

project(cpp_pipe_operator_helper
    LANGUAGES CXX
    VERSION "${cppPipeOperatorHelperVersion}"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

message(STATUS "Configuring ${PROJECT_NAME}@${PROJECT_VERSION}")
list(APPEND CMAKE_MESSAGE_INDENT "    ")

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
option(CPP_PIPE_OPERATOR_HELPER_INSTALL
    "Install library cpp_pipe_operator_helper"
    OFF
)

if (CPP_PIPE_OPERATOR_HELPER_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()

if (CPP_PIPE_OPERATOR_HELPER_USE_MODULES)
    set(CMAKE_CXX_SCAN_FOR_MODULES ON)

    if (CPP_PIPE_OPERATOR_HELPER_BUILD_SHARED)
        add_library(${PROJECT_NAME} SHARED)
        message(STATUS "Provides module with shared library.")

        if (MSVC)
            target_sources(${PROJECT_NAME} PRIVATE src/dll_dummy_export.cpp)
        endif ()
    else ()
        add_library(${PROJECT_NAME} STATIC)
        message(STATUS "Provides module with static library.")
    endif ()

    if (MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /Zc:__cplusplus)
    endif ()

    target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

    target_include_directories(${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )
    target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET ${PROJECT_NAME}_modules TYPE CXX_MODULES FILES
        modules/pipe_operator_helper.cppm
    )
else ()
    if (CPP_PIPE_OPERATOR_HELPER_BUILD_SHARED)
        message(WARNING "`CPP_PIPE_OPERATOR_HELPER_BUILD_SHARED` ignored "
            "because `CPP_PIPE_OPERATOR_HELPER_USE_MODULES` was OFF. Set "
            "`CPP_PIPE_OPERATOR_HELPER_BUILD_SHARED` OFF to avoid this warning."
        )
    endif ()

    add_library(${PROJECT_NAME} INTERFACE)
    target_include_directories(${PROJECT_NAME} INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    )

    if (MSVC)
        target_compile_options(${PROJECT_NAME} INTERFACE /Zc:__cplusplus)
    endif ()

    message(STATUS "Provides headers.")
endif ()

if (CPP_PIPE_OPERATOR_HELPER_INSTALL)
    if (MSVC AND CPP_PIPE_OPERATOR_HELPER_USE_MODULES)
        message(FATAL_ERROR "Installation of library with modules is not supported on MSVC yet!")
    endif ()

    include(install)

    if (CPP_PIPE_OPERATOR_HELPER_BUILD_TESTS)
        include(test_package)
    endif ()
endif ()
