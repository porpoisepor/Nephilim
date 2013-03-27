-- Hollow Engine Premake Script

local libname = "nephilim"
local enginename = "NephilimEngine"
local action = _ACTION or ""
local version = "0.8.3"
local samplelist = { "SampleBasic" }
	
newoption {
   trigger     = "samples",
   description = "Makes project files for the samples"
}

newoption {
	trigger		= "android",
	description = "Applicable for visual studio solutions. You need vs-android plugin."
}

solution (enginename)
	location("Build/" .. action)
	configurations { "Debug", "Release" }
	defines ( "ENGINE_VERSION_STRING=" .. version)
	defines { "MINIMAL_BUILD" }
	
	project (enginename)
		language "C++"
		kind "StaticLib"
		includedirs { "Include" , "IncludeExt"}
		files { "Source/*.cpp" }
		files { "Include/*.h" }
		targetdir("Lib")
		flags { "Unicode" }
		
		if table.contains(_ARGS, "--android") then
			if string.find(action, "vs*") then
				print("Android with Visual Studio detected, vs-android must be installed!\nChange the platform to Android and compile!")				
			end
			targetextension ".a"
		end
		
		configuration "Debug"
			defines { "_DEBUG" , "DEBUG" }
			flags { "Symbols" }
			targetname(libname .. "-d")
			
		configuration "Release"
			targetname(libname)
			flags { "Optimize" }
			
			
			
	-- Prepare the sample projects	
	if table.contains(_ARGS, "--samples")  then
	for i=1 , # samplelist do
		local sample = samplelist[i]
		project (sample)
			language "C++"
			kind "ConsoleApp"
			vpaths { ["Headers"] = "**.h" }
			vpaths { ["Source"] = "**.cpp" }
			files { "Samples/" .. sample .. "/Source/*" }
			includedirs { "Include" , "IncludeExt"}
			links (enginename)
		
			if os.get() == "windows" and not table.contains(_ARGS, "--android") then
				links("opengl32");
			end
			
			configuration "Debug"
				flags { "Symbols" }
				
			configuration "Release"			
				flags { "Optimize" }
	end
	end
	