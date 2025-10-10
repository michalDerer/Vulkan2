
#include "macross.hpp"
#include "dynamic.hpp"


void Dynamic::loadLib(const char* path)
{
    if (!lib)
    {
        lib = LoadLibraryA(path);
        NULL_CHECK(lib);

        vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(lib, "vkGetInstanceProcAddr");
        NULL_CHECK(vkGetInstanceProcAddr);

        vkCreateInstance = (PFN_vkCreateInstance)vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkCreateInstance");
        NULL_CHECK(vkCreateInstance);
        
    }
}

void Dynamic::freeLib()
{
    if (lib)
    {
        FreeLibrary(lib);
        lib = nullptr;
        
        //global level functions
        vkGetInstanceProcAddr = VK_NULL_HANDLE;
        vkCreateInstance = VK_NULL_HANDLE;
        //instance level functions
        vkDestroyInstance = VK_NULL_HANDLE;
#ifdef VK_USE_PLATFORM_WIN32_KHR
        vkCreateWin32SurfaceKHR = VK_NULL_HANDLE;
#endif
        vkDestroySurfaceKHR = VK_NULL_HANDLE;
    }
}

void Dynamic::loadInstanceLevel(VkInstance& instance)
{
    vkDestroyInstance = (PFN_vkDestroyInstance)vkGetInstanceProcAddr(instance, "vkDestroyInstance");
    NULL_CHECK(vkDestroyInstance);

#ifdef VK_USE_PLATFORM_WIN32_KHR
    vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");
    NULL_CHECK(vkCreateWin32SurfaceKHR);
#endif

    vkDestroySurfaceKHR = (PFN_vkDestroySurfaceKHR)vkGetInstanceProcAddr(instance, "vkDestroySurfaceKHR");
    NULL_CHECK(vkDestroySurfaceKHR);



}
