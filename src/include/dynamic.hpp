#pragma once

#include <iostream>

#include "vulkan/vulkan.h"


struct Dynamic
{
    public:
    // Pointer na nacitanu kniznicu. 
    // HMODULE alebo HINSTANCE je to to iste.
    HMODULE lib = nullptr;


    /*
    In Vulkan, functions are categorized into global, instance, and device level:

    Level	    How you load it
    Global	    vkGetInstanceProcAddr(nullptr, "function")
    Instance	vkGetInstanceProcAddr(instance, "function")
    Device	    vkGetDeviceProcAddr(device, "function")
    */


    //GLOBAL LEVEL FUNCTIONS
    PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = VK_NULL_HANDLE;
    PFN_vkCreateInstance vkCreateInstance = VK_NULL_HANDLE;
    //INSTANCE LEVEL FUNCTIONS
    PFN_vkDestroyInstance vkDestroyInstance = VK_NULL_HANDLE;
    //platform depending
    PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR = VK_NULL_HANDLE;
    PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR = VK_NULL_HANDLE;


    public:
    void loadLib(const char* path);
    void freeLib();

    void loadInstanceLevel(VkInstance& instance);
};