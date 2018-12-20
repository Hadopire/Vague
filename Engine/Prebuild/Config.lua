#!lua


SupportedPlatforms =
{
	["Win64"]   = { ["supported"] = true, ["suffix"] = "_Win64" },
}

SupportedConfigurations =
{
	["Release"] = { ["supported"] = true, ["suffix"] = "_r" },
	["Debug"]   = { ["supported"] = true, ["suffix"] = "_d" },
}

function GetSuffix(_platform, _config)

	return SupportedPlatforms[_platform]["suffix"] .. SupportedConfigurations[_config]["suffix"]
end

function SetWorkspacePlatforms()

	PlatformsTable = {}
	for p, desc in pairs(SupportedPlatforms) do
		if (desc["supported"] == true) then
			table.insert(PlatformsTable, p)
		end
	end
	platforms (PlatformsTable)
end

function SetWorkspaceConfigurations()

	ConfigsTable = {}
	for c, desc in pairs(SupportedConfigurations) do
		if (desc["supported"] == true) then
			table.insert(ConfigsTable, c)
		end
	end
	configurations (ConfigsTable)
end

function FilterPlatforms(_platform, _do)

	if (SupportedPlatforms[_platform]["supported"] == true) then
		filter {"platforms:".._platform}
			_do()
		filter {}
	end
end

function FilterConfigurations(_config, _do)

	if (SupportedConfigurations[_config]["supported"] == true) then
		filter {"configurations:".._config}
			_do()
		filter {}
	end
end

function ForAllSupportedPlatformsAndConfigs(_do)

	for p, supportedPlatform in pairs(SupportedPlatforms) do
		if (supportedPlatform["supported"] == true) then
			for c, supportedConfig in pairs(SupportedConfigurations) do
				if (supportedConfig["supported"] == true) then
					filter {"platforms:"..p, "configurations:"..c}
						_do(p, c)
				end
			end
		end
	end
	filter {}
end

function SetTargetSuffixes()

	ForAllSupportedPlatformsAndConfigs( function(_platform, _config)
		targetsuffix (GetSuffix(_platform, _config))
	end )
end
