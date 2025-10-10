
//#include <windows.h>
//#include <iostream>
#include <vector>

//defines pre vulkan header setnute v CMakeLists
// #define VK_NO_PROTOTYPES
// #define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"

#include "SDL3/SDL.h"
#include "SDL3/SDL_vulkan.h"

#include "macross.hpp"
#include "dynamic.hpp"



#ifdef _DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageTypes, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    std::cerr << "\nMESSAGE: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}
#endif



struct Context
{
    VkInstance instance = VK_NULL_HANDLE;
#ifdef _DEBUG
    VkDebugUtilsMessengerEXT debugUtilsMessengerEXT = VK_NULL_HANDLE;
#endif
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    VkPhysicalDevice pDvecie = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;

    int renderQueueFamilyIdx = -1;
    VkQueue queue = VK_NULL_HANDLE;
};


int main()
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


    std::vector<const char*> instanceLayers{};
    std::vector<const char*> instanceExtensions{};


#ifdef _DEBUG
    uint32_t instaLaPropertiesCount;
    VK_CHECK(d.vkEnumerateInstanceLayerProperties(&instaLaPropertiesCount, VK_NULL_HANDLE));
    std::vector<VkLayerProperties> instaLaProperties{ instaLaPropertiesCount };
    VK_CHECK(d.vkEnumerateInstanceLayerProperties(&instaLaPropertiesCount, instaLaProperties.data()));

    std::cout << "Awalyable instance layers:\n";
    for (const auto& instaLaProperty : instaLaProperties)
    {
        std::cout << instaLaProperty.layerName << " " << instaLaProperty.specVersion << "\n";

        if (strcmp(instaLaProperty.layerName, "VK_LAYER_KHRONOS_validation") == 0)
        {
            instanceLayers.push_back(instaLaProperty.layerName);
        }
    }

    std::cout << "\nAwalyable instance layer extensions:\n";
    for (const auto& instaLa : instanceLayers)
    {
        uint32_t instaLaExPropertiesCount;
        VK_CHECK(d.vkEnumerateInstanceExtensionProperties(instaLa, &instaLaExPropertiesCount, VK_NULL_HANDLE));
        std::vector<VkExtensionProperties> instaLaExProperties{ instaLaExPropertiesCount };
        VK_CHECK(d.vkEnumerateInstanceExtensionProperties(instaLa, &instaLaExPropertiesCount, instaLaExProperties.data()));

        for (const auto& instaLaExProperty : instaLaExProperties)
        {
            std::cout << instaLaExProperty.extensionName << " " << instaLaExProperty.specVersion << "\n";

            if (strcmp(instaLaExProperty.extensionName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
            {
                instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }
        }
    }
#endif


    // Get SDL-required Vulkan instance extensions mozem volat az po SDL_Init
    Uint32 arrSize = 0;
    const char* const* arr = SDL_Vulkan_GetInstanceExtensions(&arrSize);
    for (auto i = 0; i < arrSize; i++)
    {
        instanceExtensions.push_back(arr[i]);
    }


    std::cout << "\nInstance layers to activate:\n";
    for (const auto& iLa : instanceLayers)
    {
        std::cout << iLa << "\n";
    }
    std::cout << "\nInstance extension to activate:\n";
    for (const auto& iEx : instanceExtensions)
    {
        std::cout << iEx << "\n";
    }


    VkApplicationInfo appInfo{
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "Dynamic Vulkan 1.4 Example",
        .apiVersion = VK_API_VERSION_1_4 };

    VkInstanceCreateInfo instanceInfo{
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
        .enabledLayerCount = static_cast<uint32_t>(instanceLayers.size()),
        .ppEnabledLayerNames = instanceLayers.data(),
        .enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size()),
        .ppEnabledExtensionNames = instanceExtensions.data()};

    VK_CHECK(d.vkCreateInstance(&instanceInfo, VK_NULL_HANDLE, &context.instance));


    d.loadInstanceLevel(context.instance);


#ifdef _DEBUG
    VkDebugUtilsMessengerCreateInfoEXT debugInfo{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
        .pfnUserCallback = debugCallback};

    VK_CHECK(d.vkCreateDebugUtilsMessengerEXT(context.instance, &debugInfo, VK_NULL_HANDLE, &context.debugUtilsMessengerEXT));
#endif


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
        if (context.instance)
        {
            d.vkDestroyInstance(context.instance, VK_NULL_HANDLE);
            context.instance = VK_NULL_HANDLE;
        }
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
#ifdef _DEBUG
    if (context.debugUtilsMessengerEXT)
    {
        d.vkDestroyDebugUtilsMessengerEXT(context.instance, context.debugUtilsMessengerEXT, nullptr);
        context.debugUtilsMessengerEXT = VK_NULL_HANDLE;
    }
#endif
    if (context.instance)
    {
        d.vkDestroyInstance(context.instance, nullptr);
        context.instance = VK_NULL_HANDLE;
    }

    d.freeLib();

    std::cout << "DONE\n";
    return 0;
}