#pragma once

#include <iostream>

#include "vulkan/vulkan.h"


//-----------------------------------------------------------------------------


#define MEMBER(name) PFN_##name name = VK_NULL_HANDLE;


//-----------------------------------------------------------------------------


struct Dynamic
{

private:

#ifdef VK_USE_PLATFORM_WIN32_KHR
    // Pointer na nacitanu kniznicu. 
    // HMODULE alebo HINSTANCE je to to iste.
    HMODULE lib = nullptr;
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    void* lib = nullptr;
#endif

public:
    void loadLib(const char* path);

#ifdef VK_USE_PLATFORM_WIN32_KHR
    HMODULE get_lib() const;
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    void* get_lib() const;
#endif

    void freeLib();

    void loadInstanceLevelDebugUtils(VkInstance& instance);
    void loadInstanceLevel(VkInstance& instance);


//-----------------------------------------------------------------------------


    /*
    In Vulkan, functions are categorized into global, instance, and device level:

    Level	    How you load it
    Global	    vkGetInstanceProcAddr(nullptr, "function")
    Instance	vkGetInstanceProcAddr(instance, "function")
    Device	    vkGetDeviceProcAddr(device, "function")
    */


//-----------------------------------------------------------------------------


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
    
#ifdef DEBUG
    MEMBER(vkCreateDebugUtilsMessengerEXT)
    MEMBER(vkDestroyDebugUtilsMessengerEXT)
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
    MEMBER(vkCreateWin32SurfaceKHR)
    MEMBER(vkGetPhysicalDeviceWin32PresentationSupportKHR)
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
    MEMBER(vkCreateXlibSurfaceKHR)
    MEMBER(vkGetPhysicalDeviceXlibPresentationSupportKHR)
#endif

    MEMBER(vkDestroySurfaceKHR)
    MEMBER(vkCreateDevice)
    MEMBER(vkDestroyDevice)
    MEMBER(vkGetDeviceQueue2)
    MEMBER(vkGetPhysicalDeviceSurfaceSupportKHR)
    MEMBER(vkGetPhysicalDeviceSurfaceCapabilitiesKHR)
    MEMBER(vkCreateSwapchainKHR)
    MEMBER(vkDestroySwapchainKHR)
};


#undef MEMBER
