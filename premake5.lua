workspace "SlateUI"
    configurations { "Debug", "Release" }
    architecture "x64"
    include "SlateUI.lua"
    project "SlateTests"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        files { "tests/**.cpp", "tests/**.h" }
        includedirs { "include" }
        links { "SlateLib" }
