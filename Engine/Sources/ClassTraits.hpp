#pragma once


// Macros

#define VAGUE_NONCOPYABLE(_Class)               \
private:                                        \
    _Class(const _Class&) = delete;             \
    _Class& operator = (const _Class&) = delete;


#define VAGUE_DEFAULTCONSTRUCTOR_NOTALLOWED(_Class) \
protected:                                          \
    _Class() = default;


// Interface implementations

namespace Vague
{
    class INonCopyable
    {
    public:

        INonCopyable()  = default;
        ~INonCopyable() = default;

        VAGUE_NONCOPYABLE(INonCopyable);
    };

    class IDefaultConstructorCallNotAllowed
    {
        VAGUE_DEFAULTCONSTRUCTOR_NOTALLOWED(IDefaultConstructorCallNotAllowed);
    };
}
