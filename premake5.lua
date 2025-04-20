workspace "Iconoclast"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.system}/%{cfg.buildcfg}_%{cfg.architecture}"

project "Iconoclast"
    location "Iconoclast"
    kind "StaticLib"
    language "C++"
    staticruntime "on"
    
    targetdir ("%{prj.name}" .. "/bin/" .. outputdir)
    objdir ("%{prj.name}" .. "/obj/" .. outputdir)

    pchheader "IconoclastPCH.h"
    pchsource "Iconoclast/src/IconoclastPCH.cpp"

    files 
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.cpp",
    }

    vpaths
    {
        ["Header Files"] = { "**.h" },
        ["Source Files"] = { "**.cpp" },
        ["Resource Files"] = {}
    }

    includedirs
    {
        "%{prj.name}/include",
        "%{prj.name}/include/Iconoclast",
        "%{prj.name}/include/Iconoclast/Platform",
        "%{prj.name}/include/Iconoclast/Platform/DirectX",
        "%{prj.name}/include/Iconoclast/Renderer",
        "%{prj.name}/include/Iconoclast/Platform/Windows",
        "%{prj.name}/include/Iconoclast/Events"
    }

    filter "system:windows"
        cppdialect "c++20"
        staticruntime "on"
        systemversion "latest"

        defines
        {
        }

    filter "configurations:Debug"
        buildoptions "/MTd"
        symbols "on"

    filter "configurations:Release"
        buildoptions "/MT"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "on"

    targetdir ("%{prj.name}" .. "/bin/" .. outputdir)
    objdir ("%{prj.name}" .. "/obj/" .. outputdir)

    files 
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/src/**.cpp",
    }

    vpaths
    {
        ["Header Files"] = { "**.h" },
        ["Source Files"] = { "**.cpp" },
        ["Resource Files"] = {}
    }

    includedirs
    {
        "Iconoclast/include",
        "Iconoclast/include/Iconoclast",
        "Iconoclast/include/Iconoclast/Platform/DirectX",
        "Iconoclast/include/Iconoclast/Platform/Windows",
        "Iconoclast/include/Iconoclast/Renderer",
        "Iconoclast/include/Iconoclast/Events"
    }

    links
    {
        "Iconoclast",
        "dxgi.lib",
        "d3d11.lib",
        "d3dcompiler.lib"
    }

    filter "system:windows"
        cppdialect "c++20"
        staticruntime "on"
        systemversion "latest"

    filter "configurations:Debug"
        buildoptions "/MTd"
        symbols "on"

    filter "configurations:Release"
        buildoptions "/MT"
        optimize "on"
