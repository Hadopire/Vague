#include "ResourceCache/ResHandle.hpp"

#include "ResourceCache/ResCache.hpp"

namespace Vague
{
    ResHandle::ResHandle(Resource &_resource, char *_buffer, unsigned int _size, ResCache *_pResCache)
        : m_resource(_resource)
    {
        m_buffer = _buffer;
        m_size = _size;
        m_extra = nullptr;
        m_pResCache = _pResCache;
    }

    ResHandle::~ResHandle()
    {
        delete m_buffer;
        m_pResCache->MemoryHasBeenFreed(m_size);
    }
}
