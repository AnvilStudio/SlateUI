project "SlateLib"
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    staticruntime "off"
    files { "include/**.h", "src/**.cpp" }
    includedirs { "include" }
    targetdir ("bin/" .. (outdir or "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}") .. "/%{prj.name}")
    objdir ("bin-int/" .. (outdir or "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}") .. "/%{prj.name}")
