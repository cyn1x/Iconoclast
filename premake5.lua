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
    kind "SharedLib"
    language "C++"
    
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
        "%{prj.name}/include",
        "%{prj.name}/include/Iconoclast",
        "%{prj.name}/include/Iconoclast/Platform",
        "%{prj.name}/include/Iconoclast/Platform/Windows",
        "%{prj.name}/include/Iconoclast/Events"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../Sandbox/bin/" .. outputdir)
        }

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

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
        "Iconoclast/include/Iconoclast/Platform",
        "Iconoclast/include/Iconoclast/Platform/Windows",
        "Iconoclast/include/Iconoclast/Events"
    }

    links
    {
        "Iconoclast"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

    filter "configurations:Debug"
        symbols "On"

    filter "configurations:Release"
        optimize "On"
