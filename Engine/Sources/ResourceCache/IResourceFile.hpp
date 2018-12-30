#pragma once

#include <string>

#include "ResourceCache/Resource.hpp"

namespace Vague
{
    class IResourceFile
    {
    public:
        virtual bool Open()=0;
        virtual bool Contains(const Resource &_resource)=0;
        virtual int GetRawResourceSize(const Resource &_resource)=0;
        virtual int GetRawResource(const Resource &r, char *_buffer)=0;
        virtual int GetNumResources() const = 0;
        virtual std::string GetResourceName(int _num) const = 0;
        virtual ~IResourceFile() { }
    };
}
