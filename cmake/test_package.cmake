function (setTestDependencyChain)
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs DEPENDENCY_CHAIN)

    cmake_parse_arguments(PARSE_ARGV 0 arg
        "${options}" "${oneValueArgs}" "${multiValueArgs}"
    )

    if (NOT arg_DEPENDENCY_CHAIN)
        message(FATAL_ERROR "Kwarg `DEPENDENCY_CHAIN` is required")
    endif ()

    list(LENGTH arg_DEPENDENCY_CHAIN chainLength)
    math(EXPR chainEndIndex "${chainLength} - 1")
    foreach (index RANGE ${chainEndIndex})
        if (index EQUAL 0)
            continue ()
        endif ()

        math(EXPR previousIndex "${index} - 1")

        list(GET arg_DEPENDENCY_CHAIN ${index} currentTest)
        list(GET arg_DEPENDENCY_CHAIN ${previousIndex} previousTest)

        set_tests_properties("${currentTest}"
            PROPERTIES DEPENDS "${previousTest}"
        )
    endforeach ()
endfunction ()

set(testBaseName "cpp_pipe_operator_helper_package_test")

function (runTest name)
    add_test(
        NAME "${testBaseName}-${name}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${ARGN}
    )
endfunction ()

if (CMAKE_GENERATOR MATCHES "Visual Studio")
    set(configureFlags "")
    set(buildFlags --config Release)
else ()
    set(configureFlags -D CMAKE_BUILD_TYPE=Release)
    set(buildFlags "")
endif ()

file(REMOVE_RECURSE
    ${CMAKE_CURRENT_BINARY_DIR}/test_package_build
    ${CMAKE_CURRENT_BINARY_DIR}/test_package_install
    ${CMAKE_CURRENT_BINARY_DIR}/test_package_usage
)

runTest("lib-configure"
    ${CMAKE_COMMAND}
    -S . -B ${CMAKE_CURRENT_BINARY_DIR}/test_package_build
    -G "${CMAKE_GENERATOR}"
    -D CPP_PIPE_OPERATOR_HELPER_INSTALL=ON
    -D CPP_PIPE_OPERATOR_HELPER_USE_MODULES=$<BOOL:${CPP_PIPE_OPERATOR_HELPER_USE_MODULES}>
    -D CMAKE_CXX_SCAN_FOR_MODULES=$<BOOL:${CPP_PIPE_OPERATOR_HELPER_USE_MODULES}>
    -D CPP_PIPE_OPERATOR_HELPER_BUILD_SHARED=$<BOOL:${CPP_PIPE_OPERATOR_HELPER_BUILD_SHARED}>
    -D CPP_PIPE_OPERATOR_HELPER_BUILD_TESTS=OFF
    -D CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    ${configureFlags}
)
runTest("lib-build"
    ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/test_package_build ${buildFlags}
)
runTest("lib-install"
    ${CMAKE_COMMAND} --install ${CMAKE_CURRENT_BINARY_DIR}/test_package_build
    --prefix ${CMAKE_CURRENT_BINARY_DIR}/test_package_install
)

runTest("configure"
    ${CMAKE_COMMAND}
    -S ./test_package -B ${CMAKE_CURRENT_BINARY_DIR}/test_package_usage
    -G "${CMAKE_GENERATOR}"
    -D CPP_PIPE_OPERATOR_HELPER_USE_MODULES=$<BOOL:${CPP_PIPE_OPERATOR_HELPER_USE_MODULES}>
    -D cpp_pipe_operator_helper_DIR=${CMAKE_CURRENT_BINARY_DIR}/test_package_install/lib/cmake/cpp_pipe_operator_helper
    -D CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    ${configureFlags}
)
runTest("build"
    ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/test_package_usage ${buildFlags}
)
runTest("run"
    ${CMAKE_CURRENT_BINARY_DIR}/test_package_usage/cpp_pipe_operator_helper_package_test${CMAKE_EXECUTABLE_SUFFIX}
)

setTestDependencyChain(DEPENDENCY_CHAIN
    "cpp_pipe_operator_helper_test"
    "${testBaseName}-lib-configure"
    "${testBaseName}-lib-build"
    "${testBaseName}-lib-install"
    "${testBaseName}-configure"
    "${testBaseName}-build"
    "${testBaseName}-run"
)
