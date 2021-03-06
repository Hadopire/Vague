#pragma once

#include <Windows.h>

#include <unordered_map>
#include <vector>
#include <string>

#include "ResourceCache/IResourceFile.hpp"
#include "ResourceCache/Resource.hpp"

namespace Vague
{
    class ResourceDirectory : public IResourceFile
    {
    public:
        ResourceDirectory(const std::string _dirPath) { m_dirPath = _dirPath + "/"; }
        virtual ~ResourceDirectory();

        virtual bool Open();
        virtual bool Contains(const Resource &_resource);
        virtual int GetRawResourceSize(const Resource &_resource);
        virtual int GetRawResource(const Resource &_resource, char *_pBuffer);
        virtual int GetNumResources() const;
        virtual std::string GetResourceName(int _num) const;

    private:
        std::string m_dirPath;
        std::unordered_map<std::string, std::pair<HANDLE, int>> m_dirTree;
        std::vector<std::string> m_dirFiles;

        bool m_isOpen = false;
    };
}
