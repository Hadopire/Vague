#include "ResourceCache/ResCache.hpp"

#include <cassert>

#include "ResourceCache/ResHandle.hpp"
#include "ResourceCache/DefaultResourceLoader.hpp"
#include "Utils/String.hpp"

namespace Vague
{
    ResCache::ResCache(const unsigned int _sizeInMb, std::shared_ptr<IResourceFile> _resFile)
    {
        m_cacheSize = _sizeInMb * 1024 * 1024;
        m_allocated = 0;
        m_files.push_back(_resFile);
    }

    ResCache::~ResCache()
    {
        while(!m_lru.empty())
        {
            FreeOneResource();
        }
    }

    bool ResCache::Init()
    {
        bool retValue = true;
        for (auto it = m_files.begin(); it != m_files.end(); ++it)
        {
            std::shared_ptr<IResourceFile> file = *it;
            if (!file->Open())
            {
                retValue = false;
            }
        }
        RegisterLoader(std::shared_ptr<IResourceLoader>(new DefaultResourceLoader()));
        return retValue;
    }

    void ResCache::RegisterLoader(std::shared_ptr<IResourceLoader> _loader)
    {
        m_resourceLoaders.push_front(_loader);
    }

    bool ResCache::RegisterResourceFile(std::shared_ptr<IResourceFile> _file)
    {
        bool retValue = false;
        if (_file->Open())
        {
            m_files.push_back(_file);
            retValue = true;
        }
        return retValue;
    }

    std::shared_ptr<ResHandle> ResCache::GetHandle(Resource *_r)
    {
        std::shared_ptr<ResHandle> handle = Find(_r);
        if (handle==nullptr)
        {
            handle = Load(_r);
        }
        else
        {
            Update(handle);
        }
        return handle;
    }

    std::shared_ptr<ResHandle> ResCache::Load(Resource *_r)
    {
        std::shared_ptr<IResourceLoader> loader;
        std::shared_ptr<ResHandle> handle;
        std::shared_ptr<IResourceFile> file;

        for (auto it = m_resourceLoaders.begin(); it != m_resourceLoaders.end(); ++it)
        {
            std::shared_ptr<IResourceLoader> testLoader = *it;
            if (WildcardMatch(testLoader->GetPattern().c_str(), _r->m_name.c_str()))
            {
                loader = testLoader;
                break;
            }
        }

        if (!loader)
        {
            //TODO(hadopire) properly handle error
            assert(loader);
            return handle;
        }

        for (auto it = m_files.begin(); it != m_files.end(); ++it)
        {
            std::shared_ptr<IResourceFile> testFile = *it;
            if (testFile->Contains(*_r))
            {
                file = testFile;
                break;
            }
        }

        if (!file)
        {
            //TODO(hadopire) properly handle error
            assert(file);
            return handle;
        }

        unsigned int rawSize = file->GetRawResourceSize(*_r);
        char *rawBuffer = loader->UseRawFile() ? Allocate(rawSize) : new char[rawSize];

        if (rawBuffer == nullptr)
        {
            //resource cache out of memory
            return std::shared_ptr<ResHandle>();
        }
        file->GetRawResource(*_r, rawBuffer);
        char *buffer = nullptr;
        unsigned int size = 0;

        if (loader->UseRawFile())
        {
            buffer = rawBuffer;
            handle = std::shared_ptr<ResHandle>(new ResHandle(*_r, buffer, rawSize, this));
        }
        else
        {
            size = loader->GetLoadedResourceSize(rawBuffer, rawSize);
            buffer = Allocate(size);
            if (rawBuffer == nullptr || buffer == nullptr)
            {
                //resource cache out of memory
                return std::shared_ptr<ResHandle>();
            }
            handle = std::shared_ptr<ResHandle>(new ResHandle(*_r, buffer, size, this));
            bool success = loader->LoadResource(rawBuffer, rawSize, handle);
            delete rawBuffer;

            if (!success)
            {
                //resource cache out of memory
                return std::shared_ptr<ResHandle>();
            }
        }

        if (handle != nullptr)
        {
            m_lru.push_front(handle);
            m_resources[_r->m_name] = handle;
        }
        return handle;
    }

    std::shared_ptr<ResHandle> ResCache::Find(Resource *_r)
    {
        auto it = m_resources.find(_r->m_name);
        if (it == m_resources.end())
            return std::shared_ptr<ResHandle>();

        return it->second;
    }

    void ResCache::Update(std::shared_ptr<ResHandle> _handle)
    {
        m_lru.remove(_handle);
        m_lru.push_front(_handle);
    }

    char *ResCache::Allocate(unsigned int size)
    {
        if (!MakeRoom(size))
            return nullptr;

        char *mem = new char[size];
        if (mem)
            m_allocated += size;

        return mem;
    }

    bool ResCache::MakeRoom(unsigned int size)
    {
        if (size > m_cacheSize)
        {
            return false;
        }

        while (size > (m_cacheSize - m_allocated))
        {
            // The cache is empty, but there's still not enough room.
            if (m_lru.empty())
                return false;

            FreeOneResource();
        }

        return true;
    }

    void ResCache::FreeOneResource()
    {
        auto gonner = m_lru.end();
        gonner--;

        std::shared_ptr<ResHandle> handle = *gonner;

        m_lru.pop_back();
        m_resources.erase(handle->m_resource.m_name);
    }

    // This does not actually free any memory, since the
    // resource might still be in use
    void ResCache::Free(std::shared_ptr<ResHandle> _gonner)
    {
        m_lru.remove(_gonner);
        m_resources.erase(_gonner->m_resource.m_name);
    }

    // This is called whenever the memory associated with
    // a resource is actually freed
    void ResCache::MemoryHasBeenFreed(unsigned int _size)
    {
        m_allocated -= _size;
    }

    int ResCache::Preload(const std::string pattern, void (*progressCallBack)(int, bool*))
    {
        int loaded = 0;
        bool cancel = false;

        for (auto it = m_files.begin(); it != m_files.end() && !cancel; ++it)
        {
            std::shared_ptr<IResourceFile> file = *it;
            int numFiles = file->GetNumResources();
            for (int i = 0; i < numFiles && !cancel; ++i)
            {
                Resource resource(file->GetResourceName(i));

                if (WildcardMatch(pattern.c_str(), resource.m_name.c_str()))
                {
                    std::shared_ptr<ResHandle> handle = GetHandle(&resource);
                    ++loaded;
                }

                if (progressCallBack != nullptr)
                {
                    progressCallBack(i * 100 / numFiles, &cancel);
                }
            }
        }
        return loaded;
    }
}
