set_xmakever("3.0.5")

local project_name = "cpp_pipe_operator_helper"
set_project(project_name)
set_version("0.0.0")

option(project_name .. "-use-modules", function ()
    set_description("Use C++20 modules for library `" .. project_name .. "`.")
    set_default(false)
end)

option(project_name .. "-build-shared", function ()
    set_description("Build cpp_pipe_operator_helper as a shared library if " ..
        "`cpp_pipe_operator_helper-use-modules` is true."
    )
    set_default(false)
end)

option(project_name .. "-enable-test", function ()
    set_description("Test project `" .. project_name .. "` could runing with XMake " ..
        "or not. If you need running unit test, please use CMake with " ..
        "`-DCPP_PIPE_OPERATOR_HELPER_BUILD_TESTS=ON`"
    )
    set_default(false)
end)

target(project_name, function ()
    on_config(function (target)
        if get_config(project_name .. "-use-modules") then
            if get_config(project_name .. "-build-shared") then
                print("[%s]: provide modules with shared library.", project_name)
                target:set("kind", "shared")
            else
                print("[%s]: provide modules with static library.", project_name)
                target:set("kind", "static")
            end

            target:add("files", "pipe_operator_helper.cppm", {public = true})
        else
            print("[%s]: provide headers.", project_name)
            if get_config(project_name .. "-build-shared") then
                local warn_str = format("${yellow}WARNING${clear}: [%s]: Config `%s-build-shared` " ..
                    "ignored because `%s-use-modules` was false. Set `%s-build-shared` to " ..
                    "false to avoid this warning.", project_name, project_name, project_name,
                    project_name
                )
                cprint(warn_str)
            end
            target:set("kind", "headeronly")
        end
    end)

    add_includedirs("include", {public = true})
end)

if get_config(project_name .. "-enable-test") then
    target(project_name .. "_test", function ()
        set_kind("binary")

        add_defines("TEST_USE_MODULE")
        add_files("test/xmake_test/xmake_test.cpp")
        set_languages("cxx20")
        add_deps(project_name)
    end)
end
