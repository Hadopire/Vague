#pragma once

#include <string>

#include "ResourceCache/Resource.hpp"

namespace Vague
{
    class IResourceFile
    {
    public:
        virtual bool Open()=0;
        virtual int GetRawResourceSize(const Resource &r)=0;
        virtual int GetRawResource(const Resource &r, char *buffer)=0;
        virtual int GetNumResources() const = 0;
        virtual std::string GetResourceName(int num) const = 0;
        virtual ~IResourceFile() { }
    };
}
