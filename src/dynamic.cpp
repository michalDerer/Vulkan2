
#include "macros.hpp"
#include "dynamic.hpp"


#define MEMBER(name) PFN_##name name = VK_NULL_HANDLE;

#define GET_INSTA_PROC_ADR_G(name)                                          \
do {                                                                        \
    name = (PFN_##name)vkGetInstanceProcAddr(VK_NULL_HANDLE, #name);        \
    NULL_CHECK(name)                                                        \
} while (0);

#define GET_INSTA_PROC_ADR(handle, name)                                    \
do {                                                                        \
    name = (PFN_##name)vkGetInstanceProcAddr(handle, #name);                \
    NULL_CHECK(name)                                                        \
} while (0);


void Dynamic::loadLib(const char* path)
{
    if (!lib)
    {
        lib = LoadLibraryA(path);
        NULL_CHECK(lib)

        vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(lib, "vkGetInstanceProcAddr");
        NULL_CHECK(vkGetInstanceProcAddr)
        
        GET_INSTA_PROC_ADR_G(vkEnumerateInstanceLayerProperties) 
        GET_INSTA_PROC_ADR_G(vkEnumerateInstanceExtensionProperties)
        GET_INSTA_PROC_ADR_G(vkCreateInstance)
    }
}

HMODULE Dynamic::get_lib() const
{
    return lib;
}

void Dynamic::freeLib()
{
    if (lib)
    {
        FreeLibrary(lib);
        lib = nullptr;
        
        //GLOBAL LEVEL FUNCTIONS
        MEMBER(vkGetInstanceProcAddr)
        MEMBER(vkEnumerateInstanceLayerProperties)
        MEMBER(vkEnumerateInstanceExtensionProperties)
        MEMBER(vkCreateInstance)
        //INSTANCE LEVEL FUNCTIONS
        MEMBER(vkDestroyInstance)
        MEMBER(vkEnumeratePhysicalDevices)
        MEMBER(vkGetPhysicalDeviceProperties2)
        MEMBER(vkGetPhysicalDeviceQueueFamilyProperties2)
#ifdef _DEBUG
        MEMBER(vkCreateDebugUtilsMessengerEXT)
        MEMBER(vkDestroyDebugUtilsMessengerEXT)
#endif
#ifdef VK_USE_PLATFORM_WIN32_KHR
        MEMBER(vkCreateWin32SurfaceKHR)
        MEMBER(vkGetPhysicalDeviceWin32PresentationSupportKHR)
#endif
        MEMBER(vkDestroySurfaceKHR)
        MEMBER(vkCreateDevice)
        MEMBER(vkDestroyDevice)
        MEMBER(vkGetDeviceQueue2)
        MEMBER(vkGetPhysicalDeviceSurfaceSupportKHR)
        MEMBER(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
        MEMBER(vkCreateSwapchainKHR)
        MEMBER(vkDestroySwapchainKHR)
    }
}

void Dynamic::loadInstanceLevel(VkInstance& instance)
{
    GET_INSTA_PROC_ADR(instance, vkDestroyInstance)
    GET_INSTA_PROC_ADR(instance, vkEnumeratePhysicalDevices)
    GET_INSTA_PROC_ADR(instance, vkGetPhysicalDeviceProperties2)
    GET_INSTA_PROC_ADR(instance, vkGetPhysicalDeviceQueueFamilyProperties2)

#ifdef _DEBUG
    GET_INSTA_PROC_ADR(instance, vkCreateDebugUtilsMessengerEXT)
    GET_INSTA_PROC_ADR(instance, vkDestroyDebugUtilsMessengerEXT)
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
    GET_INSTA_PROC_ADR(instance, vkCreateWin32SurfaceKHR)
    GET_INSTA_PROC_ADR(instance, vkGetPhysicalDeviceWin32PresentationSupportKHR)
#endif

    GET_INSTA_PROC_ADR(instance, vkDestroySurfaceKHR)
    GET_INSTA_PROC_ADR(instance, vkCreateDevice)
    GET_INSTA_PROC_ADR(instance, vkDestroyDevice)
    GET_INSTA_PROC_ADR(instance, vkGetDeviceQueue2)
    GET_INSTA_PROC_ADR(instance, vkGetPhysicalDeviceSurfaceSupportKHR)
    GET_INSTA_PROC_ADR(instance, vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
    GET_INSTA_PROC_ADR(instance, vkCreateSwapchainKHR)
    GET_INSTA_PROC_ADR(instance, vkDestroySwapchainKHR)
}


#undef MEMBER
#undef GET_INSTA_PROC_ADR_G
#undef GET_INSTA_PROC_ADR