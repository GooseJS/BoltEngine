project "GLFW"
    location "../../lib/glfw"
    kind "StaticLib"
    language "C"

	targetdir ("build/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("build/bin-obj/" .. outputDir .. "/%{prj.name}")

	files
	{
        "%{prj.location}/src/context.c",
        "%{prj.location}/src/init.c",
        "%{prj.location}/src/input.c",
        "%{prj.location}/src/monitor.c",
        "%{prj.location}/src/vulkan.c",
        "%{prj.location}/src/window.c"
    }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "system:windows"
        systemversion "latest"
        staticruntime "off"

        files {
            "%{prj.location}/src/win32_init.c",
            "%{prj.location}/src/win32_joystick.c",
            "%{prj.location}/src/win32_monitor.c",
            "%{prj.location}/src/win32_time.c",
            "%{prj.location}/src/win32_thread.c",
            "%{prj.location}/src/win32_window.c",
            "%{prj.location}/src/wgl_context.c",
            "%{prj.location}/src/egl_context.c",
            "%{prj.location}/src/osmesa_context.c"
        }

		defines {
            "_GLFW_WIN32",
            "_CRT_SECURE_NO_WARNINGS"
		}