workspace "2022AIE-PhysicsRendering"  
    configurations { "Debug", "Release" }
    startproject "RenderingDemonstration"
    platforms { "x64" }

    filter { "platforms:x64" }
    architecture "x64"

    libdirs { "Libraries/lib" }
    includedirs { "Libraries/include"}
    cppdialect "C++17"

    

project "EngineCore" 
    defines { "ENGINECORE" }
    kind "StaticLib" 
    language "C++"
    location "EngineCore"
    targetdir "Libraries/lib" 
    files { "EngineCore/**.h", "EngineCore/**.cpp" }
    
    links { "freetype", "glew32", "glew32s", "glfw3", "opengl32" }

    filter "configurations:Debug"
    defines { "DEBUG" }  
    symbols "On" 
    filter "configurations:Release"  
    defines { "NDEBUG" }    
    optimize "On" 

project "RenderingDemonstration"
    defines { "DEMONSTRATION" }
    kind "ConsoleApp" 
    language "C++"
    location "RenderingDemonstration"
    local outputDir = "bin/%{cfg.buildcfg}"
    targetdir(outputDir)
    files { "RenderingDemonstration/**.h", "RenderingDemonstration/**.cpp" }

    includedirs { "EngineCore" }
    links { "EngineCore" }

    local libPath = "Libraries/lib/dynamic"
    local extension = ".dll"

    postbuildcommands
    {
        "{COPYFILE} \"$(SolutionDir)/" .. libPath .. "/glew32" .. extension .. "\" \"$(SolutionDir)/" .. outputDir .. "/glew32" .. extension .. "\"",
        "{COPYFILE} \"$(SolutionDir)/" .. libPath .. "/glfw3" .. extension .. "\" \"$(SolutionDir)/" .. outputDir .. "/glfw3" .. extension .. "\"",
        "{COPYDIR} \"$(SolutionDir)/RenderingDemonstration/Resources\" \"$(SolutionDir)/" .. outputDir .. "/Resources\""
    }

    filter "configurations:Debug"
    defines { "DEBUG" }  
    symbols "On" 
    filter "configurations:Release"  
    defines { "NDEBUG" }    
    optimize "On" 

