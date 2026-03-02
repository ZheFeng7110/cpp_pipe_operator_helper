include_guard()

block ()
    function (testFindGTest)
        if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/googletest")
            message(STATUS "Found GTest in subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/googletest)")

            set(BUILD_GMOCK OFF CACHE INTERNAL "" FORCE)
            set(INSTALL_GTEST OFF CACHE INTERNAL "" FORCE)
            add_subdirectory(googletest)

            return()
        endif ()

        find_package(GTest)
        if (GTest_FOUND)
            message(STATUS "Found GTest: \nGTEST_INCLUDE_DIRS=${GTEST_INCLUDE_DIRS}\n"
                "GTEST_LIBRARIES=${GTEST_LIBRARIES}\n"
                "GTEST_MAIN_LIBRARIES=${GTEST_MAIN_LIBRARIES}"
            )
        else ()
            message(STATUS "googletest not found locally, downloading with FetchContent...")

            include(FetchContent)

            FetchContent_Declare(
                googletest
                GIT_REPOSITORY https://github.com/google/googletest.git
                GIT_TAG v1.17.0
            )

            set(BUILD_GMOCK OFF CACHE INTERNAL "" FORCE)
            set(INSTALL_GTEST OFF CACHE INTERNAL "" FORCE)

            FetchContent_MakeAvailable(googletest)

            message(STATUS "googletest downloaded and available as gtest and gtest_main targets")
        endif ()
    endfunction ()

    testFindGTest()
endblock ()
