#pragma once


//////////////////////////////////////////////////////////
// !! Keep this file standalone, without any include !! //
//////////////////////////////////////////////////////////


// Config

// See https://sourceforge.net/p/predef/wiki/Home/

#if defined (_WIN32)

#   define __VAGUE_WINDOWS__

#endif


// Misc

#define VAGUE_STR2(_s)                  #_s
#define VAGUE_STR(_s)                   VAGUE_STR2(_s)


// Asserts (empty in Release build)

namespace Vague
{
    void Break();
    void PrintAssertInfos(const char* _szExpression, const char* _szFilePath, int _iLine, const char* _szFunction);
}

#if defined (__VAGUE_DEBUG__)

#   define VAGUE_FAILED_ASSERTION(_expr)\
    {                                   \
        Vague::PrintAssertInfos(VAGUE_STR(_expr), __FILE__, __LINE__, __FUNCTION__);\
        Vague::Break();                 \
    }

#   define VAGUE_ASSERT(_expr)          \
    {                                   \
        if (!(_expr))                   \
        {                               \
            VAGUE_FAILED_ASSERTION(_expr);\
        }                               \
    }

#else

#   define VAGUE_FAILED_ASSERTION(expr) do {} while(0)
#   define VAGUE_ASSERT(expr)           do {} while(0)

#endif

#define VAGUE_STATIC_ASSERT_MSG         static_assert
#define VAGUE_STATIC_ASSERT(_expr)      VAGUE_STATIC_ASSERT_MSG((_expr), "")


// Verifications (used in Release build)

#define VAGUE_VERIFY(_expr)             \
{                                       \
    if (!(_expr))                       \
    {                                   \
        VAGUE_FAILED_ASSERTION(_expr);  \
    }                                   \
}

#define VAGUE_VERIFY_RETURN(_expr, _ret)\
{                                       \
    if (!(_expr))                       \
    {                                   \
        VAGUE_FAILED_ASSERTION(_expr);  \
        return _ret;                    \
    }                                   \
}


// Programmers infos

#if defined (__VAGUE_WINDOWS__)

#   define VAGUE_TODO(_who, _what)      __pragma(message(__FILE__ "(" VAGUE_STR(__LINE__) ") : TODO (" _who "): " _what))

#else

#   define VAGUE_TODO(_s)

#endif

#define VAGUE_NOT_IMPLEMENTED(_who, _mess)    { VAGUE_TODO(_who, _mess); VAGUE_FAILED_ASSERTION(_mess); }


// Utilities

#define VAGUE_DELETE(_ptr)              { delete   _ptr; _ptr = nullptr; }
#define VAGUE_DELETE_ARRAY(_ptr)        { delete[] _ptr; _ptr = nullptr; }
#define VAGUE_SAFE_DELETE(_ptr)         { if (_ptr != nullptr) { VAGUE_DELETE(_ptr); } }
#define VAGUE_SAFE_DELETE_ARRAY(_ptr)   { if (_ptr != nullptr) { VAGUE_DELETE_ARRAY(_ptr); } }

#define VAGUE_TEST_FLAG(_v, _f)         (((_v) & (_f)) == (_f))
#define VAGUE_ASSERT_FLAG(_v, _f)       VAGUE_ASSERT(VAGUE_TEST_FLAG(_v, _f))
