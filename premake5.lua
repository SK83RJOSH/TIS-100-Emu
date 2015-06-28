solution "Tiscord"
	configurations { "Debug", "Release" }

	project "Core"
		kind "StaticLib"
		language "C++"
		targetdir "bin/%{cfg.buildcfg}"

		includedirs { "Core/include/" }
		files { "Core/include/**.hpp", "Core/src/**.cpp" }

		filter "configurations:Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

		filter "configurations:Release"
			defines { "NDEBUG" }
			optimize "On"