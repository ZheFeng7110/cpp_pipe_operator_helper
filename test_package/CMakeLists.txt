cmake_minimum_required(VERSION 3.28...4.0)

project(cpp_pipe_operator_helper_package_test CXX)

find_package(cpp_pipe_operator_helper REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE cpp_pipe_operator_helper)

if (CPP_PIPE_OPERATOR_HELPER_USE_MODULES)
    target_compile_definitions(${PROJECT_NAME} PRIVATE TEST_USE_MODULE)
endif ()
