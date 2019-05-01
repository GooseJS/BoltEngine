workspace "Bolt"
	startproject "BoltEditor"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "lib/glfw/include"
IncludeDir["GLEW"] = "lib/glew/include"
IncludeDir["spdlog"] = "lib/spdlog/include"
IncludeDir["glm"] = "lib/glm"
IncludeDir["stb"] = "lib/stb"
IncludeDir["imgui"] = "lib/imgui"
IncludeDir["json"] = "lib/json/include"
IncludeDir["debugDraw"] = "lib/debug-draw"
IncludeDir["libnoise"] = "lib/libnoise/src"
IncludeDir["noiseutils"] = "lib/libnoise/noiseutils"
IncludeDir["taskflow"] = "lib/cpp-taskflow"

include "premake/glfw"
include "premake/glew"
include "premake/imgui"
include "premake/libnoise"

project "BoltEngine"
	location "BoltEngine"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir("build/bin/"..outputDir.."/%{prj.name}")
	objdir("build/bin-obj/"..outputDir.."/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.bullet}",
		"%{IncludeDir.json}",
		"%{IncludeDir.debugDraw}",
		"%{IncludeDir.libnoise}",
		"%{IncludeDir.noiseutils}",
		"%{IncludeDir.taskflow}",
		"%{prj.name}/src"
	}

	links
	{
		"GLFW",
		"GLEW",
		"ImGui",
		"libnoise",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		disablewarnings {
			"4251",
			"4267",
			"4244",
			"4005"
		}

		defines
		{
			"BOLT_WIN32",
			"BOLT_BUILD_DLL",
			"NOISE_STATIC"
		}

	filter "configurations:Debug"
		defines "BOLT_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "BOLT_RELEASE"
		runtime "Release"
		optimize "On"

project "BoltEditor"
	location "BoltEditor"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"
	debugdir "data"

	targetdir("build/bin/"..outputDir.."/%{prj.name}")
	objdir("build/bin-obj/"..outputDir.."/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"BoltEngine/src",
		"%{IncludeDir.stb}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLEW}",
		"%{IncludeDir.bullet}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.json}",
		"%{IncludeDir.debugDraw}",
		"%{IncludeDir.libnoise}",
		"%{IncludeDir.noiseutils}",
		"%{IncludeDir.taskflow}",
		"%{prj.name}/src"
	}

	links
	{
		"GLEW",
		"GLFW",
		"ImGui",
		"opengl32.lib",
		"BoltEngine"
	}

	defines
	{
		"NOISE_STATIC"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		disablewarnings {
			"4251",
			"4267",
			"4244",
			"4005"
		}

		defines
		{
			"BOLT_WIN32",
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../data"),
		}

	filter "configurations:Debug"
		defines "APOLLO_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "APOLLO_RELEASE"
		runtime "Release"
		optimize "On"
