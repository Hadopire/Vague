#pragma once

#include "Resource.h"
#include "IResourceExtraData.h"

#include <memory>

class ResHandle
{
    friend class ResCache;
protected:
    Resource m_resource;
    char *m_buffer;
    unsigned int m_size;
    std::shared_ptr<IResourceExtraData> m_extra;
    ResCache *m_pResCache;

public:
    ResHandle(Resource &_resource, char *_buffer, unsigned int _size, ResCache *_pResCache);
    virtual ~ResHandle();

    const std::string GetName() const { return m_resource.m_name; }
    unsigned int Size() const { return m_size; }
    char *Buffer() const { return m_buffer; }
    char *WritableBuffer() { return m_buffer; }

    std::shared_ptr<IResourceExtraData> GetExtra() { return m_extra; }
    void SetExtra(std::shared_ptr<IResourceExtraData> _extra) { m_extra = _extra; }
};
