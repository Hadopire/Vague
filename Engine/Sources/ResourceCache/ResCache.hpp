#pragma once

#include <list>
#include <map>
#include <string>
#include <memory>

#include "ResourceCache/IResourceLoader.hpp"
#include "ResourceCache/IResourceFile.hpp"
#include "ResourceCache/Resource.hpp"

namespace Vague
{
    class ResCache
    {
        friend class ResHandle;
    public:
        ResCache(const unsigned int _sizeInMb, std::shared_ptr<IResourceFile> _resFile);
        ~ResCache();

        bool Init();
        void RegisterLoader(std::shared_ptr<IResourceLoader> _loader);
        bool RegisterResourceFile(std::shared_ptr<IResourceFile> _file);

        std::shared_ptr<ResHandle> GetHandle(Resource *_r);
        int Preload(const std::string pattern, void (*progressCallBack)(int, bool*));
        void Flush(void);
    protected:
        std::list<std::shared_ptr<ResHandle>> m_lru; // LRU (least recently used)
        std::map<std::string, std::shared_ptr<ResHandle>> m_resources;
        std::list<std::shared_ptr<IResourceLoader>> m_resourceLoaders;

        std::list<std::shared_ptr<IResourceFile>> m_files;

        unsigned int m_cacheSize;
        unsigned int m_allocated;

        std::shared_ptr<ResHandle> Find(Resource *_r);
        void Update(std::shared_ptr<ResHandle> _handle);
        std::shared_ptr<ResHandle> Load(Resource *_r);
        void Free(std::shared_ptr<ResHandle> _gonner);

        bool MakeRoom(unsigned int _size);
        char *Allocate(unsigned int _size);
        void FreeOneResource();
        void MemoryHasBeenFreed(unsigned int _size);
    };
}
