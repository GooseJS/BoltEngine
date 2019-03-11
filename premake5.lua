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
IncludeDir["Box2D"] = "lib/Box2D"

include "premake/Box2D"
include "premake/glfw"
include "premake/glew"
include "premake/imgui"

project "BoltEngine"
	location "BoltEngine"
	kind "SharedLib"
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
		"%{IncludeDir.Box2D}",
		"%{prj.name}/src"
	}

	links
	{
		"GLFW",
		"GLEW",
		"Box2D",
		"ImGui",
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
			"BOLT_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../data"),
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
		"%{IncludeDir.Box2D}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.spdlog}",
		"%{prj.name}/src"
	}

	links
	{
		"GLEW",
		"GLFW",
		"Box2D",
		"ImGui",
		"opengl32.lib",
		"BoltEngine"
	}

	defines
	{
		""
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
