#include "ResourceCache/Win32_ResourceDirectory.hpp"

#include <Windows.h>

#include <algorithm>
#include <cctype>
#include <locale>
#include <codecvt>

namespace Vague
{
    bool ResourceDirectory::Open()
    {
        if (m_isOpen)
            return true;

        std::string pattern = m_dirPath + "*";
        std::wstring_convert<std::codecvt_utf8_utf16 <wchar_t>, wchar_t> converter;
        std::wstring wPattern = converter.from_bytes(pattern.data());
        WIN32_FIND_DATAW findData;
        HANDLE searchHandle = FindFirstFileW(wPattern.data(), &findData);

        if (searchHandle == INVALID_HANDLE_VALUE)
        {
            //TODO(Hadopire) Properly handle error
            return false;
        }

        std::wstring wDirPath = converter.from_bytes(m_dirPath.data());
        do
        {
            if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                std::wstring wFilePath = wDirPath + findData.cFileName;
                HANDLE fileHandle = CreateFileW(
                        wFilePath.data(),
                        GENERIC_READ,
                        FILE_SHARE_READ,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
                if (fileHandle == INVALID_HANDLE_VALUE)
                {
                    //TODO(Hadopire) Properly handle error
                    return false;
                }

                std::string utf8FileName = converter.to_bytes(findData.cFileName);
                std::transform(utf8FileName.begin(), utf8FileName.end(), utf8FileName.begin(), ((int (*)(int))std::tolower));
                m_dirTree.insert({utf8FileName, std::make_pair(fileHandle, findData.nFileSizeLow)});
                m_dirFiles.push_back(utf8FileName);
            }
        } while (FindNextFileW(searchHandle, &findData));

        if (GetLastError() != ERROR_NO_MORE_FILES)
        {
            //TODO(Hadopire) Properly handle error
        }
        else
        {
            m_isOpen = true;
        }

        return m_isOpen;
    }

    bool ResourceDirectory::Contains(const Resource &_resource)
    {
        return (bool)m_dirTree.count(_resource.m_name);
    }

    int ResourceDirectory::GetRawResourceSize(const Resource &_resource)
    {
        return m_dirTree[_resource.m_name].second;
    }

    int ResourceDirectory::GetRawResource(const Resource &_resource, char *_pBuffer)
    {
        std::pair<HANDLE, int> file = m_dirTree[_resource.m_name];
        DWORD read;

        BOOL success = ReadFile(
                file.first,
                _pBuffer,
                file.second,
                &read,
                NULL);
        if (!success)
        {
            //TODO(Hadopire) Properly handle error
            return 0;
        }

        return read;
    }

    int ResourceDirectory::GetNumResources() const
    {
        return (int)m_dirTree.size();
    }

    std::string ResourceDirectory::GetResourceName(int _num) const
    {
        return m_dirFiles[_num];
    }

    ResourceDirectory::~ResourceDirectory()
    {
        //TODO(Hadopire) Delete stuff
    }
}
