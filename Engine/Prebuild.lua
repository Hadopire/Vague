#!lua


-- Engine Project

Engine = {}
Engine.Dir  = path.getdirectory(".").."/"
Engine.Name = "Engine"
Vague.ProjectCommonSettings(Engine.Dir, Engine.Name, "StaticLib")


-- Tests Projects

Test = {}
Test.CommonDir = Engine.Dir..Vague.Dir.Tests

-- ResourceCache

Test.Name = "ResourceCache"
Test.Dir  = Test.CommonDir..Test.Name.."/"
Vague.ProjectCommonSettings(Test.Dir, Test.Name, "WindowedApp")
Vague.LibProjectDependency(Engine.Name, Engine.Name, Engine.Dir)