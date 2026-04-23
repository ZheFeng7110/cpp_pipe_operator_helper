set_xmakever("3.0.5")

local project_name = "cpp_pipe_operator_helper"
set_project(project_name)
set_version("1.0.0")

option("modules", function ()
    set_description("Use C++20 modules for library `" .. project_name .. "`.")
    set_default(false)
end)

option("test", function ()
    set_description("Build tests for " .. project_name .. ".")
    set_default(false)
end)

target(project_name, function ()
    on_config(function (target)
        if get_config("modules") then
            print("[%s]: provide modules.", project_name)
            target:set("kind", "moduleonly")
            target:add("files", "modules/pipe_operator_helper.cppm", {public = true})
        else
            print("[%s]: provide headers.", project_name)
            target:set("kind", "headeronly")
        end
    end)

    add_includedirs("include", {public = true})
end)

if get_config("test") then
    includes("test")
end
