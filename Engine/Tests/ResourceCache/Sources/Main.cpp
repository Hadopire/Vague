#include <Tests/Tests.hpp>

#include <FileSystem/Filesystem.hpp>
#include <ResourceCache/ResourceDirectory.hpp>
#include <ResourceCache/Resource.hpp>
#include <ResourceCache/ResCache.hpp>
#include <ResourceCache/ResHandle.hpp>


static const std::string g_assetPath = "/../Assets";
static const int g_assetCount = 4;

namespace Vague
{
    u32 ResourceDirectoryTest()
    {
        VAGUE_UTEST_INIT;

        ResourceDirectory directory(GetExecDir() + g_assetPath);
        Resource dataIntegrity("dataintegrity");

        bool success = directory.Open();
        VAGUE_UTEST(success);

        int numResources = directory.GetNumResources();
        VAGUE_UTEST(numResources == g_assetCount);

        int size = directory.GetRawResourceSize(dataIntegrity);
        VAGUE_UTEST(size == 4096);

        int buffer[1024];
        success = directory.GetRawResource(dataIntegrity, (char*)buffer);
        VAGUE_UTEST(success);

        for (int i = 0; i < 1024; ++i)
        {
            if (buffer[i] != i)
            {
                success = false;
                break;
            }
        }
        VAGUE_UTEST(success);

        VAGUE_UTEST_END;
    }

    u32 ResCacheTest()
    {
        VAGUE_UTEST_INIT;

        ResourceDirectory *directory = new ResourceDirectory(GetExecDir() + g_assetPath);
        ResCache cache(1, directory);

        bool success = cache.Init();
        VAGUE_UTEST(success);

        Resource nullA("500kBnullA");
        Resource nullB("500kBnullB");
        Resource nullC("500kBnullC");

        {
            // load 500kB into the 1mB cache
            std::shared_ptr<ResHandle> hdA = cache.GetHandle(&nullA);
            VAGUE_UTEST(hdA != nullptr);

            // load another 500kB for a total of 1mB
            std::shared_ptr<ResHandle> hdB = cache.GetHandle(&nullB);
            VAGUE_UTEST(hdB != nullptr);

            // try to load another 500kB
            // the cache if full so this should fail
            std::shared_ptr<ResHandle> hdC = cache.GetHandle(&nullC);
            VAGUE_UTEST(hdC == nullptr);
        }

        std::weak_ptr<ResHandle> weakhdC;

        {
            // the cache should have room now
            std::shared_ptr<ResHandle> hdC = cache.GetHandle(&nullC);
            VAGUE_UTEST(hdC != nullptr);

            weakhdC = hdC;
        }

        {
            // our previous data should be cached
            VAGUE_UTEST(weakhdC.expired() == false);

            // load 500kB into the 1mB cache
            std::shared_ptr<ResHandle> hdA = cache.GetHandle(&nullA);
            VAGUE_UTEST(hdA != nullptr);

            // load another 500kB for a total of 1mB
            std::shared_ptr<ResHandle> hdB = cache.GetHandle(&nullB);
            VAGUE_UTEST(hdB != nullptr);

            // the cache made room for assets a and b
            // so our previous data should have been deleted
            VAGUE_UTEST(weakhdC.expired() == true);
        }

        VAGUE_UTEST_END;
    }
}

int main()
{
    Vague::ResourceDirectoryTest();
    Vague::ResCacheTest();
    return 0;
}
