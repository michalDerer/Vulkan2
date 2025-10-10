
//#include <windows.h>
#include <iostream>
//#include <vector>

//definovane v nastaveniach
// #define VK_NO_PROTOTYPES
// #define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"

#include "SDL3/SDL.h"
#include "SDL3/SDL_vulkan.h"

#include "macross.hpp"


// #ifndef _DEBUG
// #define VK_CHECK(value)  
// #else
// #define VK_CHECK(value)                                                                                     \
// do {                                                                                                        \
//     VkResult vValue = value;                                                                                \
//     if (vValue != VK_SUCCESS)                                                                               \
//     {                                                                                                       \
//         /*fprintf(stderr, "Vulkan error: %d at %s:%d\n", vValue, __FILE__, __LINE__);*/                     \
//         std::cerr << "Vulkan error: " << vValue << " at " << __FILE__ << ":" << __LINE__ << std::endl;      \
//         abort();                                                                                            \
//     }                                                                                                       \
// } while (0)                                                                                            
// #endif

// #ifndef _DEBUG
// #define NULL_CHECK(p)  
// #else
// #define NULL_CHECK(p)                                                                                       \
// do {                                                                                                        \
//     void* pP = p;                                                                                           \
//     if (pP == NULL)                                                                                         \
//     {                                                                                                       \
//         std::cerr << "Null error at " << __FILE__ << ":" << __LINE__ << std::endl;                          \
//         abort();                                                                                            \
//     }                                                                                                       \
// } while (0)
// #endif






struct Context
{
    VkInstance instance = VK_NULL_HANDLE;
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkPhysicalDevice pDvecie = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;

    int renderQueueFamilyIdx = -1;
    VkQueue queue = VK_NULL_HANDLE;
};


int main ()
{
    //HINSTANCE vulkanLib = LoadLibrary("vulkan-1.dll");
    //NULL_CHECK(vulkanLib);

    PFN_vkGetInstanceProcAddr fvkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vulkanLib, "vkGetInstanceProcAddr");
    NULL_CHECK(fvkGetInstanceProcAddr);

    PFN_vkCreateInstance fvkCreateInstance = VK_NULL_HANDLE;
    PFN_vkDestroyInstance fvkDestroyInstance = VK_NULL_HANDLE;

    /*
    In Vulkan, functions are categorized into global, instance, and device level:

    Level	    How you load it
    Global	    vkGetInstanceProcAddr(nullptr, "function")
    Instance	vkGetInstanceProcAddr(instance, "function")
    Device	    vkGetDeviceProcAddr(device, "function")
    */

    fvkCreateInstance = (PFN_vkCreateInstance)fvkGetInstanceProcAddr(VK_NULL_HANDLE, "vkCreateInstance");
    NULL_CHECK(fvkCreateInstance);



    Context context{};
    //VkInstance instance = VK_NULL_HANDLE;

 

    VkApplicationInfo appInfo{ 
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Dynamic Vulkan 1.4 Example",
        .apiVersion = VK_API_VERSION_1_4 };

    VkInstanceCreateInfo instanceInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo};

    VK_CHECK(fvkCreateInstance(&instanceInfo, VK_NULL_HANDLE, &context.instance));

    fvkDestroyInstance = (PFN_vkDestroyInstance)fvkGetInstanceProcAddr(context.instance, "vkDestroyInstance");
    NULL_CHECK(fvkDestroyInstance);



    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        std::cerr << "SDL_Init Error: " << __FILE__ << ":" << __LINE__  << " " << SDL_GetError() << std::endl;
    }
    else
    {

        //SDL_Vulkan_LoadLibrary();
        //SDL_Vulkan_UnloadLibrary();
        //SDL_Vulkan_CreateSurface
        //SDL_Vulkan_DestroySurface

        // Query SDL-required Vulkan extensions mozem volat az po SDL_Init
        Uint32 extensionsCount = 0;
        const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&extensionsCount);
        for (Uint32 i = 0; i < extensionsCount; i++)
        {
            std::cout << extensions[i] << "\n";
        }


        //SDL_Window* window = SDL_CreateWindow("MyApp", 800, 600, SDL_WINDOW_VULKAN);
        //// Get window properties
        //SDL_PropertiesID props = SDL_GetWindowProperties(window);
        //// Get HWND
        //HWND hwnd = (HWND)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
        //// Get HINSTANCE (the module handle)
        //HINSTANCE hinstance = (HINSTANCE)SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER, nullptr);
        

        // Create Vulkan-capable SDL window
        SDL_Window* window = SDL_CreateWindow(
            "Vulkan SDL3 Window",
            800,
            600,
            SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
        );
    
        if (!window) 
        {
            std::cerr << "SDL_CreateWindow Error: " << __FILE__ << ":" << __LINE__  << " " << SDL_GetError() << std::endl;
        }
        else
        {
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
        }
    
        SDL_DestroyWindow(window);
        SDL_Quit();
    }




    if (context.instance)
    {
        fvkDestroyInstance(context.instance, VK_NULL_HANDLE);
        context.instance = VK_NULL_HANDLE;
    }

    // if (vulkanLib)
    // {
    //     FreeLibrary(vulkanLib);
    // }

    std::cout << "DONE\n";
}