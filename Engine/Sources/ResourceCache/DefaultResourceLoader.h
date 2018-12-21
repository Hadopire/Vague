#pragma once

#include "IResourceLoader.h"

class DefaultResourceLoader : public IResourceLoader
{
    virtual std::string GetPattern() { return "*"; }
    virtual bool UseRawFile() { return true; }
    virtual unsigned int GetLoadedResourceSize(char *_rawBuffer, unsigned int _rawSize) { return _rawSize; }
    virtual bool LoadResource(char *_rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> _handle) { return true; }
};
