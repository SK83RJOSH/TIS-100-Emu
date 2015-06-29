solution "Tiscord"
	configurations { "Debug", "Release" }

	project "Core"
		kind "StaticLib"
		language "C++"
		targetdir "bin/%{cfg.buildcfg}"

		configuration { "gmake" }
			buildoptions { "-std=c++11" }

		includedirs { "Core/include/" }
		files { "Core/include/**.hpp", "Core/src/**.cpp" }

		filter "configurations:Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On"

	project "BytecodeTest"
		kind "ConsoleApp"
		language "C++"
		targetdir "bin/%{cfg.buildcfg}"

		configuration { "gmake" }
			buildoptions { "-std=c++11" }

		includedirs { "Core/include/", "BytecodeTest/include/" }
		links { "Core" }
		files { "BytecodeTest/include/**.hpp", "BytecodeTest/src/**.cpp" }

		filter "configurations:Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On"
