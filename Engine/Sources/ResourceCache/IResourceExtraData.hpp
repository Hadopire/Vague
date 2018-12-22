#pragma once

#include <string>

namespace Vague
{
    class IResourceExtraData
    {
    public:
        virtual std::string ToString()=0;
    };
}
