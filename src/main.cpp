
//#include <windows.h>
//#include <iostream>
//#include <vector>

//defines pre vulkan header setnute v CMakeLists
// #define VK_NO_PROTOTYPES
// #define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"

#include "SDL3/SDL.h"
#include "SDL3/SDL_vulkan.h"

#include "macross.hpp"
#include "dynamic.hpp"



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
   
    Dynamic d{};
    d.loadLib("vulkan-1.dll");

    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "SDL_Init Error: " << __FILE__ << ":" << __LINE__ << " " << SDL_GetError() << std::endl;

        d.freeLib();
        return 1;
    }

    Context context{};

    // get SDL-required Vulkan instance extensions mozem volat az po SDL_Init
    Uint32 instanceExtensionsCount = 0;
    const char* const* instanceExtensions = SDL_Vulkan_GetInstanceExtensions(&instanceExtensionsCount);
    for (Uint32 i = 0; i < instanceExtensionsCount; i++)
    {
        std::cout << instanceExtensions[i] << "\n";
    }

    VkApplicationInfo appInfo{ 
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Dynamic Vulkan 1.4 Example",
        .apiVersion = VK_API_VERSION_1_4 };

    VkInstanceCreateInfo instanceInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = instanceExtensionsCount,
        .ppEnabledExtensionNames = instanceExtensions };

    VK_CHECK(d.vkCreateInstance(&instanceInfo, VK_NULL_HANDLE, &context.instance));

    d.loadInstanceLevel(context.instance);

    // Create Vulkan-capable SDL window
    SDL_Window* window = SDL_CreateWindow(
        "Vulkan SDL3 Window",
        800,
        600,
        SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE
    );

    if (!window)
    {
        std::cerr << "SDL_CreateWindow Error: " << __FILE__ << ":" << __LINE__ << " " << SDL_GetError() << std::endl;

        SDL_Quit();
        d.vkDestroyInstance(context.instance, VK_NULL_HANDLE);
        context.instance = VK_NULL_HANDLE;
        d.freeLib();
        return 1;
    }
    
    // Get window properties
    SDL_PropertiesID window_props = SDL_GetWindowProperties(window);
    HWND hwnd = (HWND)SDL_GetPointerProperty(window_props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
    HINSTANCE hinstance = (HINSTANCE)SDL_GetPointerProperty(window_props, SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER, nullptr);

    VkWin32SurfaceCreateInfoKHR surfaceInfo{
        .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
        .hinstance = hinstance,
        .hwnd = hwnd
    };
    
    VK_CHECK(d.vkCreateWin32SurfaceKHR(context.instance, &surfaceInfo, VK_NULL_HANDLE, &context.surface));




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
  

    if (context.surface)
    {
        d.vkDestroySurfaceKHR(context.instance, context.surface, nullptr);
        context.surface = VK_NULL_HANDLE;
    }
    if (context.instance)
    {
        d.vkDestroyInstance(context.instance, nullptr);
        context.instance = VK_NULL_HANDLE;
    }

    d.freeLib();

    std::cout << "DONE\n";
    return 0;
}