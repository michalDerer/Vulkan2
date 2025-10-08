
//#include <windows.h>
#include <iostream>
//#include <vector>

//definovane v CMakeLists.txt
// #define VK_NO_PROTOTYPES
// #define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"

#include "SDL3/SDL.h"
#include "SDL3/SDL_vulkan.h"

//Use a Loader Library volk namiesto rucneho loadu metod

void main ()
{
    HMODULE vulkanLib = LoadLibraryA("vulkan-1.dll");
    if (!vulkanLib) 
    {
        std::cerr << "Failed to load vulkan-1.dll" << std::endl;
        return;
    }

    PFN_vkGetInstanceProcAddr fvkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vulkanLib, "vkGetInstanceProcAddr");
    PFN_vkCreateInstance fvkCreateInstance = VK_NULL_HANDLE;
    PFN_vkDestroyInstance fvkDestroyInstance = VK_NULL_HANDLE;

    if (!fvkGetInstanceProcAddr) 
    {
        std::cerr << "Failed to load vkGetInstanceProcAddr" << std::endl;
        return;
    }

    /*
    In Vulkan, functions are categorized into global, instance, and device level:

    Level	    How you load it
    Global	    vkGetInstanceProcAddr(nullptr, "function")
    Instance	vkGetInstanceProcAddr(instance, "function")
    Device	    vkGetDeviceProcAddr(device, "function")
    */

    fvkCreateInstance = (PFN_vkCreateInstance)fvkGetInstanceProcAddr(VK_NULL_HANDLE, "vkCreateInstance");    

    if (!fvkCreateInstance) 
    {
        std::cerr << "Failed to load vkGetInstanceProcAddr" << std::endl;
        return;
    }


    VkInstance instance = VK_NULL_HANDLE;
    
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Dynamic Vulkan Example";
    appInfo.applicationVersion = 1;
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = 1;
    appInfo.apiVersion = VK_API_VERSION_1_1;

    VkInstanceCreateInfo instanceCreateInfo{};
    instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pApplicationInfo = &appInfo;

    VkResult result = fvkCreateInstance(&instanceCreateInfo, VK_NULL_HANDLE, &instance);

    if (result != VK_SUCCESS) 
    {
        std::cerr << "vkCreateInstance failed with code " << result << std::endl;
        return;
    }
    

    fvkDestroyInstance = (PFN_vkDestroyInstance)fvkGetInstanceProcAddr(instance, "vkDestroyInstance");

    if (!fvkDestroyInstance)
    {
        std::cerr << "Failed to load vkDestroyInstance" << std::endl;
        return;
    }



    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << "\n";
        goto end;
    }

    // Create Vulkan-capable SDL window
    SDL_Window* window = SDL_CreateWindow(
        "Vulkan SDL3 Window",
        800,
        600,
        SDL_WINDOW_VULKAN
    );

    if (!window) 
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << "\n";
        SDL_Quit();
        
        goto end;
    }
    
    // Query SDL-required Vulkan extensions
    Uint32 extensionsCount = 0;
    const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionsCount);

    for(Uint32 i = 0; i < extensionsCount; i++ )
    {
        std::cout << extensions[i] << "\n";
    }

    // Main loop
    bool running = true;
    SDL_Event event;
    while (running) 
    {
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_EVENT_QUIT) 
            {
                running = false;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();


    end:

    if (instance)
    {
        fvkDestroyInstance(instance, VK_NULL_HANDLE);
        instance = VK_NULL_HANDLE;
    }

    if (vulkanLib)
    {
        FreeLibrary(vulkanLib);
    }

    std::cout << "DONE\n";
}