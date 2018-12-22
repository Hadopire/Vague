#!lua


require("Vague")


workspace ("Vague")

    location ("../"..Vague.Dir.Build)
    Vague.SetWorkspacePlatforms()
    Vague.SetWorkspaceConfigurations()

    Vague.FilterPlatforms("Win64", function()
        architecture    "x64"
    end )
    filter {}

    Vague.FilterConfigurations("Release", function()
        defines         {"__VAGUE_RELEASE__=1"}
    end )
    Vague.FilterConfigurations("Debug", function()
        defines         {"__VAGUE_DEBUG__=1"}
    end )
    filter {}


    -- Engine project
    Vague.RequirePrebuild("../Engine/")
