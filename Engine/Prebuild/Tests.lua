#!lua


function DefineTestsProject(_projectName, _projectDir, _projectToTestName, _projectToTestDir)

	project (_projectName)
		location (BuildDir.._projectDir)

		filter {}
			kind		"WindowedApp"
			language	"C++"

			targetdir	(BinDir)
			debugdir	(RootDir)

			CommonCppFileSettings(_projectDir..SourcesDir)

			includedirs	{EngineDir..SourcesDir, _projectToTestDir..SourcesDir}

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
