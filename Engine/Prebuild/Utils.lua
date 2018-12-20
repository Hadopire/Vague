#!lua


function CommonCppFileSettings(_sourcesDir)

	includedirs	{_sourcesDir}
	files		{_sourcesDir.."**.h",
				 _sourcesDir.."**.hpp",
				 _sourcesDir.."**.c",
				 _sourcesDir.."**.cpp",
				 _sourcesDir.."**.tpp",
				 _sourcesDir.."**.inl"}
	vpaths		{["*"] = _sourcesDir}
	excludes	{_sourcesDir.."**.bak"}
end
