project "libnoise"
    location "../../lib/libnoise"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("build/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("build/bin-obj/" .. outputDir .. "/%{prj.name}")

	files {
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/noiseutils/noiseutils.cpp"
    }

    includedirs {
        "%{prj.location}/src/",
        "%{prj.location}/src/noise",
        "%{prj.location}/noiseutils/"
    }

	defines {
		"NOISE_STATIC"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"
