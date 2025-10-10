
#include "macross.hpp"
#include "dynamic.hpp"

void Dynamic::loadLib(const char *path)
{
    if (!lib)
    {
        lib = LoadLibrary(path);
        NULL_CHECK(lib);
    }
}

void Dynamic::freeLib()
{
    if (lib)
    {
        FreeLibrary(lib);
        lib = nullptr;
    }
}
