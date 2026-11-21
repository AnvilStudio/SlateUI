-- Use this script's location rather than the calling workspace directory.
-- Premake evaluates included scripts inside the parent workspace.
local slateRoot = path.getdirectory(_SCRIPT)
local slateOutput = outdir or "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "SlateLib"
    location (slateRoot)
    kind "StaticLib"
    language "C++"
    cppdialect "C++20"
    warnings "off"
    staticruntime "off"

    targetdir (path.join(slateRoot, "bin", slateOutput, "%{prj.name}"))
    objdir (path.join(slateRoot, "bin-int", slateOutput, "%{prj.name}"))

    files {
        path.join(slateRoot, "include/**.h"),
        path.join(slateRoot, "src/**.cpp")
    }
    includedirs {
        path.join(slateRoot, "include")
    }
