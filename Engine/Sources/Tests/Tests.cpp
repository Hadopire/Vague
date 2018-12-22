
#include "Tests.hpp"

#include "Output.hpp"


namespace Vague
{
    namespace Tests
    {
        void TestFailLog(const char* const _szFilePath, const int _iLine, const char* const _szFunction)
        {
            VAGUE_COUT << "Test: Fail: " << _szFilePath << ", " << _iLine << ", " << _szFunction << std::endl;
        }

        void TestEndLog(const char* _szFunction, const u32 _iTestCount, const u32 _iSuccessful)
        {
            if (_iTestCount == _iSuccessful)
            {
                VAGUE_COUT << "Test: Succes: " << _szFunction << " (" << _iSuccessful << '/' << _iTestCount << ')' << std::endl;
            }
            else
            {
                VAGUE_COUT << "Test: FAIL: " << _szFunction << " | " << _iTestCount - _iSuccessful << " failed tests (" << _iSuccessful << '/' << _iTestCount << ')' << std::endl;
            }
        }
    }
}
