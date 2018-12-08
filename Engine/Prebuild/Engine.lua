#!lua


function DefineEngineProject(_projectName, _projectKind)

	project (_projectName)
		location (BuildDir.._projectName.."/")

		filter {}
			kind		(_projectKind)
			language	"C++"

			targetdir	(BinDir)
			debugdir	(RootDir)
			targetname	(_projectName)

			CommonCppFileSettings(SourcesDir)

			libdirs		{}
			flags 		{"NoMinimalRebuild", "MultiProcessorCompile"}

			--LinkExternalLibs()

			SetTargetSuffixes()

		FilterConfigurations("Release", function()
			warnings	"Extra"
			optimize	"Full"
			flags		{"FatalWarnings"}
		end )
		FilterConfigurations("Debug", function()
			symbols		"On"
		end )
		filter {}
end

function LinkEngineLib()

	ForAllSupportedPlatformsAndConfigs( function(_platform, _config)
		dependson	{ EngineName }
		links		{ EngineName..GetSuffix(_platform, _config) }
	end )
	filter {}
end
