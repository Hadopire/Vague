#pragma once

#include "ResHandle.h"

#include <string>

class IResourceLoader
{
  public:
    virtual std::string GetPattern()=0;
    virtual bool UseRawFile()=0;
    virtual unsigned int GetLoadedResourceSize(char *_rawBuffer, unsigned int _rawSize)=0;
    virtual bool LoadResource(char *_rawBuffer, unsigned int rawSize, std::shared_ptr<ResHandle> _handle)=0;
};
