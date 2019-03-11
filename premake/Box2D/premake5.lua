project 'Box2D'
	kind 'StaticLib'
	systemversion "latest"
	location "../../lib/Box2D"
	files {
		'%{prj.location}/Box2D/**.cpp', '%{prj.location}/Box2D/**.h',
	}
	includedirs { '%{prj.location}' }
	language "C++"
	symbols 'On'
	warnings 'Extra'
    cppdialect 'C++11'

	targetdir ("build/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("build/bin-obj/" .. outputDir .. "/%{prj.name}")

	filter 'configurations:Debug'
	 	defines { 'DEBUG', '_CRT_SECURE_NO_WARNINGS' }
		optimize 'Off'
	filter 'configurations:Release'
		defines { 'NDEBUG' }
		optimize 'On'
	filter {}