#!lua


-- Main "namespace"

Vague = {}


-- Common directories ----------------------------------------------------------

Vague.Dir =
{
    Assets = "Assets/",
    Bin = "Bin/",
    Build = "Build/",
    Docs = "Docs/",
    External = "External/",
    Sources = "Sources/",
    Tests = "Tests/",
}

Vague.Prebuild = "Prebuild"

Vague.RequirePrebuild = function (_projectDir)

    require (_projectDir..Vague.Prebuild)
end


-- Configuration ---------------------------------------------------------------

Vague.SupportedPlatforms =
{
    ["Win64"]   = { supported = true, suffix = "_Win64" },
}

Vague.SupportedConfigurations =
{
    ["Release"] = { supported = true, suffix = "_r" },
    ["Debug"]   = { supported = true, suffix = "_d" },
}

Vague.GetSuffix = function (_platform, _config)

    return Vague.SupportedPlatforms[_platform].suffix .. Vague.SupportedConfigurations[_config].suffix
end

Vague.SetWorkspacePlatforms = function()

    PlatformsTable = {}
    for p, desc in pairs(Vague.SupportedPlatforms) do
        if (desc.supported == true) then
            table.insert(PlatformsTable, p)
        end
    end
    platforms (PlatformsTable)
end

Vague.SetWorkspaceConfigurations = function()

    ConfigsTable = {}
    for c, desc in pairs(Vague.SupportedConfigurations) do
        if (desc.supported == true) then
            table.insert(ConfigsTable, c)
        end
    end
    configurations (ConfigsTable)
end

Vague.FilterPlatforms = function(_platform, _do)

    if (Vague.SupportedPlatforms[_platform].supported == true) then
        filter {"platforms:".._platform}
            _do()
        filter {}
    end
end

Vague.FilterConfigurations = function(_config, _do)

    if (Vague.SupportedConfigurations[_config].supported == true) then
        filter {"configurations:".._config}
            _do()
        filter {}
    end
end

Vague.ForAllSupportedPlatformsAndConfigs = function(_do)

    for p, supportedPlatform in pairs(Vague.SupportedPlatforms) do
        if (supportedPlatform.supported == true) then
            for c, supportedConfig in pairs(Vague.SupportedConfigurations) do
                if (supportedConfig.supported == true) then
                    filter {"platforms:"..p, "configurations:"..c}
                        _do(p, c)
                end
            end
        end
    end
    filter {}
end

Vague.SetTargetSuffixes = function()

    Vague.ForAllSupportedPlatformsAndConfigs( function(_platform, _config)
        targetsuffix (Vague.GetSuffix(_platform, _config))
    end )
end


-- Utilities -------------------------------------------------------------------

Vague.CommonCppFileSettings = function(_sourcesDir)

    includedirs {_sourcesDir}
    files       {_sourcesDir.."**.h",
                 _sourcesDir.."**.hpp",
                 _sourcesDir.."**.c",
                 _sourcesDir.."**.cpp",
                 _sourcesDir.."**.tpp",
                 _sourcesDir.."**.inl"}
    vpaths      {["*"] = _sourcesDir}
    excludes    {_sourcesDir.."**.bak"}
end

Vague.SetProjectKind = function(_projectKind)

    kind (_projectKind)

    if (_projectKind ~= "StaticLib") then
        entrypoint "mainCRTStartup"
    end
end

Vague.ProjectCommonSettings = function(_projectDir, _projectName, _projectKind)

    project (_projectName)

        location (_projectDir..Vague.Dir.Build)

        filter {}
            language    "C++"
            Vague.SetProjectKind(_projectKind)

            targetname  (_projectName)
            targetdir   (_projectDir..Vague.Dir.Bin)
            debugdir    (_projectDir)
            Vague.SetTargetSuffixes()

            Vague.CommonCppFileSettings(_projectDir..Vague.Dir.Sources)

            libdirs     {}
            flags       {"NoMinimalRebuild", "MultiProcessorCompile"}

        Vague.FilterConfigurations("Release", function()
            warnings    "Extra"
            optimize    "Full"
            --flags       {"FatalWarnings"}
        end )
        Vague.FilterConfigurations("Debug", function()
			warnings    "Default"
            symbols     "On"
        end )
        filter {}
end

Vague.LibProjectDependency = function(_projectName, _libName, _projectDir)

    libdirs         {_projectDir..Vague.Dir.Bin}

    Vague.ForAllSupportedPlatformsAndConfigs( function(_platform, _config)
        dependson   { _projectName }
        links       { _projectName..Vague.GetSuffix(_platform, _config) }
    end )

    includedirs     {_projectDir..Vague.Dir.Sources}
    filter {}
end
