#pragma once

#include "IResourceLoader.hpp"
#include "IResourceFile.hpp"
#include "Resource.hpp"

#include <list>
#include <map>
#include <string>
#include <memory>

namespace Vague
{
    class ResCache
    {
        friend class ResHandle;
    public:
        ResCache(const unsigned int _sizeInMb, IResourceFile *_resFile);
        ~ResCache();

        bool Init();
        void RegisterLoader(std::shared_ptr<IResourceLoader> loader);

        std::shared_ptr<ResHandle> GetHandle(Resource *_r);
        int Preload(const std::string pattern, void (*progressCallBack)(int, bool*));
        void Flush(void);
    protected:
        std::list<std::shared_ptr<ResHandle>> m_lru; // LRU (least recently used)
        std::map<std::string, std::shared_ptr<ResHandle>> m_resources;
        std::list<std::shared_ptr<IResourceLoader>> m_resourceLoaders;

        //TODO(hadopire) allow the cache to use multiple resource files
        IResourceFile *m_file;

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
