
#include <Core.hpp>

#include <intrin.h> // __debugbreak()

#include <Output.hpp>


namespace Vague
{
    void Break()
    {
        __debugbreak();
    }

    void PrintAssertInfos(const char* const _szExpression, const char* const _szFilePath, const int _iLine, const char* const _szFunction)
    {
        VAGUE_COUT << "############## ASSERT FAILED ##############\n";
        VAGUE_COUT << "File:     " << _szFilePath << " (" << _iLine << ")\n";
        VAGUE_COUT << "Fonction: " << _szFunction << "\n\n";
        VAGUE_COUT << _szExpression << "\n\n";
        VAGUE_COUT << "###########################################" << std::endl;
    }
}
