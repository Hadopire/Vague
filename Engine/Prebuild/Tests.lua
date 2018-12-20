#!lua


function DefineTestProject(_projectName, _projectDir, _projectToTestName, _projectToTestDir)

	project (_projectName)
		location (BuildDir.._projectName.."/")

		filter {}
			kind		"WindowedApp"
			language	"C++"

			targetdir	(BinDir)
			debugdir	(RootDir)
			targetname	(_projectName)

			CommonCppFileSettings(_projectDir.."Sources/") -- TODO : changer la maniere d'appeler les sources

			--includedirs	{_projectToTestDir.."Sources/"} -- TODO : changer la maniere d'appeler les sources
			includedirs	{SourcesDir} -- TODO : changer la maniere d'appeler les sources

			libdirs		{BinDir}
			flags 		{"NoMinimalRebuild", "MultiProcessorCompile"}

			LinkEngineLib()

			SetTargetSuffixes()

			entrypoint 	"mainCRTStartup"

		FilterConfigurations("Release", function()
			warnings	"Extra"
			optimize	"Full"
			flags		{"FatalWarnings"}
		end )
		FilterConfigurations("Debug", function()
			symbols		"On"
		end )
		filter {}

		ForAllSupportedPlatformsAndConfigs( function(_platform, _config)
			links (_projectToTestName..GetSuffix(_platform, _config))
		end )
		filter {}
end
