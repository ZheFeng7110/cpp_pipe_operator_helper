add_requires("gtest v1.17.0", {configs = {gmock = false}})

target("cpp_pipe_operator_helper_test", function ()
    set_kind("binary")
    set_languages("cxx20")
    set_policy("build.c++.modules", true)

    if has_config("vs") then
        add_cxxflags("/Zc:__cplusplus")
    end

    add_packages("gtest")
    add_deps("cpp_pipe_operator_helper")

    add_defines("TEST_USE_MODULE")
    add_files("*.cpp", "pipe_operators_tests/*.cpp")
    add_includedirs("./")
end)
