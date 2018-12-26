#include "FileSystem/FileSystem.hpp"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef WIN32_LEAN_AND_MEAN

#include <cctype>
#include <locale>
#include <codecvt>

#include "Core.hpp"

namespace Vague
{
    const std::string &GetExecDir()
    {
        static std::string execPath;
        if (execPath.size() == 0)
        {
            HMODULE hModule = GetModuleHandleW(NULL);
            WCHAR path[MAX_PATH];
            DWORD len = GetModuleFileNameW(hModule, path, MAX_PATH);
            VAGUE_ASSERT(len != 0);

            while (path[len] != L'\\')
                --len;
            path[len] = (WCHAR)0;

            std::wstring_convert<std::codecvt_utf8_utf16 <wchar_t>, wchar_t> converter;
            execPath = converter.to_bytes(path);
        }
        return execPath;
    }
}
