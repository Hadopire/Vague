
#include <Tests/Tests.hpp>

namespace Vague
{
    u32 DummyTest()
    {
        VAGUE_UTEST_INIT;

        u32 i = 0;
        VAGUE_UTEST(i == 0);

        u32 j = 0;
        VAGUE_UTEST(i == j);

        VAGUE_UTEST_END;
    }
}

int main()
{
    Vague::DummyTest();
	return 0;
}
