#!lua

require("Config")
require("Directories")
require("Utils")
require("External")
require("Engine")
require("Tests")


workspace ("Vague")

	location (BuildDir)
	SetWorkspacePlatforms()
	SetWorkspaceConfigurations()

	FilterPlatforms("Win64", function()
		architecture	"x64"
	end )
	filter {}

	FilterConfigurations("Release", function()
		defines			{"__VAGUE_RELEASE__=1"}
	end )
	FilterConfigurations("Debug", function()
		defines			{"__VAGUE_DEBUG__=1"}
	end )
	filter {}

	-- Engine
	EngineName = "Engine"
	EngineDir  = RootDir..EngineName.."/"
	DefineEngineProject(EngineName, "StaticLib")

	-- Tests
	TestsResourcesName = "TestsResourceCache"
	TestsResourcesDir  = TestsDir.."ResourceCache".."/"
	DefineTestProject(TestsResourcesName, TestsResourcesDir, EngineName, EngineDir)
