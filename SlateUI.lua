project "SlateLib"
    location "."
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    warnings "off"
    staticruntime "off"

    targetdir("bin/" .. outdir .. "/%{prj.name}")
    objdir("bin-int/" .. outdir .. "/%{prj.name}")

    files {
        "./include/**.h",
        "./src/**.cpp"
    }

    includedirs {
        "./include"
    }

project "Tests"
    location "./tests"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    warnings "off"
    staticruntime "off"

    targetdir("bin/" .. outdir .. "/%{prj.name}")
    objdir("bin-int/" .. outdir .. "/%{prj.name}")

    files {
        "./tests/**.h",
        "./tests/**.cpp"
    }

    includedirs {
        "./include"
    }

    links {
        "SlateLib"
    }