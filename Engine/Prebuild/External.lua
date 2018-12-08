#!lua


LinkExternalLibFunctions =
{
}

function LinkExternalLibs(...)

	for k, lib in pairs({...}) do
		LinkExternalLibFunctions[lib]()
	end
end
