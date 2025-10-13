
//#include <windows.h>
//#include <iostream>
#include <vector>
#include <string>

//defines pre vulkan header setnute v CMakeLists
// #define VK_NO_PROTOTYPES
// #define VK_USE_PLATFORM_WIN32_KHR
#include "vulkan/vulkan.h"

#include "SDL3/SDL.h"
#include "SDL3/SDL_vulkan.h"

#include "macros.hpp"
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
    VkSwapchainKHR swapchain = VK_NULL_HANDLE;

    VkPhysicalDevice pDvecie = VK_NULL_HANDLE;
    int pDeviceRenderQueueFamilyIdx = -1;

    VkDevice device = VK_NULL_HANDLE;
    VkQueue queue = VK_NULL_HANDLE;
};


std::string apiVersionToString(uint32_t apiVersion)
{
    std::string s{std::to_string(VK_API_VERSION_VARIANT(apiVersion))}; 
    //s.append(std::to_string(VK_API_VERSION_VARIANT(apiVersion)));
    s.append(".");
    s.append(std::to_string(VK_API_VERSION_MAJOR(apiVersion)));
    s.append(".");
    s.append(std::to_string(VK_API_VERSION_MINOR(apiVersion)));
    s.append(".");
    s.append(std::to_string(VK_API_VERSION_PATCH(apiVersion)));

    return s;
}

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
    VK_CHECK(d.vkEnumerateInstanceLayerProperties(&instaLaPropertiesCount, VK_NULL_HANDLE))
    std::vector<VkLayerProperties> instaLaProperties{ instaLaPropertiesCount };
    VK_CHECK(d.vkEnumerateInstanceLayerProperties(&instaLaPropertiesCount, instaLaProperties.data()))

    std::cout << "Awalyable instance layers:\n";
    for (const auto& instaLaProperty : instaLaProperties)
    {
        std::cout << instaLaProperty.layerName << " " <<  apiVersionToString(instaLaProperty.specVersion) << "\n";

        if (strcmp(instaLaProperty.layerName, "VK_LAYER_KHRONOS_validation") == 0)
        {
            instanceLayers.push_back(instaLaProperty.layerName);
        }
    }

    std::cout << "\nAwalyable instance layer extensions:\n";
    for (const auto& instaLa : instanceLayers)
    {
        uint32_t instaLaExPropertiesCount;
        VK_CHECK(d.vkEnumerateInstanceExtensionProperties(instaLa, &instaLaExPropertiesCount, VK_NULL_HANDLE))
        std::vector<VkExtensionProperties> instaLaExProperties{ instaLaExPropertiesCount };
        VK_CHECK(d.vkEnumerateInstanceExtensionProperties(instaLa, &instaLaExPropertiesCount, instaLaExProperties.data()))

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

    VK_CHECK(d.vkCreateInstance(&instanceInfo, VK_NULL_HANDLE, &context.instance))


    d.loadInstanceLevel(context.instance);


#ifdef _DEBUG
    VkDebugUtilsMessengerCreateInfoEXT debugInfo{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT,
        .pfnUserCallback = debugCallback};

    VK_CHECK(d.vkCreateDebugUtilsMessengerEXT(context.instance, &debugInfo, VK_NULL_HANDLE, &context.debugUtilsMessengerEXT))
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
    
    VK_CHECK(d.vkCreateWin32SurfaceKHR(context.instance, &surfaceInfo, VK_NULL_HANDLE, &context.surface))


    // Get physical device
    {
        uint32_t phDevicesCount;
        VK_CHECK(d.vkEnumeratePhysicalDevices(context.instance, &phDevicesCount, VK_NULL_HANDLE))
        std::vector<VkPhysicalDevice> phDevices{ phDevicesCount };
        VK_CHECK(d.vkEnumeratePhysicalDevices(context.instance, &phDevicesCount, phDevices.data()))

        for (const auto& phDevice : phDevices)
        {
            uint32_t phDeviceQFamilyPropsCount;
            d.vkGetPhysicalDeviceQueueFamilyProperties2(phDevice, &phDeviceQFamilyPropsCount, VK_NULL_HANDLE);
            std::vector<VkQueueFamilyProperties2> phDeviceQFamilyProps{ phDeviceQFamilyPropsCount, {
                .sType = VK_STRUCTURE_TYPE_QUEUE_FAMILY_PROPERTIES_2 } };
            d.vkGetPhysicalDeviceQueueFamilyProperties2(phDevice, &phDeviceQFamilyPropsCount, phDeviceQFamilyProps.data());
            
            VkPhysicalDeviceProperties2 phDeviceProperties{
                .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2};
            d.vkGetPhysicalDeviceProperties2(phDevice, &phDeviceProperties);

            std::cout << "\n" << phDeviceProperties.properties.deviceName << " " << apiVersionToString(phDeviceProperties.properties.apiVersion) << "\n";
            for (int i = 0; i < phDeviceQFamilyProps.size(); i++)
            {
                VkBool32 surfaceSupported = false;
                VK_CHECK(d.vkGetPhysicalDeviceSurfaceSupportKHR(phDevice, i, context.surface, &surfaceSupported))           //To determine whether a queue family of a physical device supports presentation to a given surface
                VkBool32 surfaceSupportedWin = d.vkGetPhysicalDeviceWin32PresentationSupportKHR(phDevice, i);               //To determine whether a queue family of a physical device supports presentation to the Microsoft Windows desktop
                VkBool32 graphicsBIT = phDeviceQFamilyProps[i].queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT;
                VkBool32 transferBIT = phDeviceQFamilyProps[i].queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT;
                bool selected = false;

                if (surfaceSupported && surfaceSupportedWin && graphicsBIT && transferBIT && context.pDvecie == VK_NULL_HANDLE)
                {
                    context.pDvecie = phDevice;
                    context.pDeviceRenderQueueFamilyIdx = i;
                    selected = true;
                }

                std::cout << "Family idx: " << i << 
                    " family count: " << phDeviceQFamilyProps[i].queueFamilyProperties.queueCount << 
                    " surfaceSupported: " << ((surfaceSupported) ? "true" : "false") <<
                    " surfaceSupportedWin: " << ((surfaceSupportedWin) ? "true" : "false") <<
                    " graphics BIT: " << ((graphicsBIT) ? "true" : "false") <<
                    " transfer BIT: " << ((transferBIT) ? "true" : "false") <<
                    (selected ? " SELECTED" : "") <<
                    "\n";
            }
        }
    }

    // Create device with queue and get queue handle
    {
        float queuePriorities[1] = {1.0f};

        VkDeviceQueueCreateInfo cQueueInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = static_cast<uint32_t>(context.pDeviceRenderQueueFamilyIdx),
            .queueCount = 1,
            .pQueuePriorities = queuePriorities};

        VkDeviceCreateInfo deviceInfo{
            .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
            .queueCreateInfoCount = 1,
            .pQueueCreateInfos = &cQueueInfo};

        VK_CHECK(d.vkCreateDevice(context.pDvecie, &deviceInfo, VK_NULL_HANDLE, &context.device))

        VkDeviceQueueInfo2 gQueueInfo {
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_INFO_2,
            .queueFamilyIndex = static_cast<uint32_t>(context.pDeviceRenderQueueFamilyIdx),
            .queueIndex = 0};

        d.vkGetDeviceQueue2(context.device, &gQueueInfo, &context.queue);
    }

    // Create Swapchain
    {
        VkSurfaceCapabilitiesKHR surfaceCapabilities{};
        VK_CHECK(d.vkGetPhysicalDeviceSurfaceCapabilitiesKHR(context.pDvecie, context.surface, &surfaceCapabilities))

        std::cout << "\nExtend:" << surfaceCapabilities.currentExtent.width << "x" << surfaceCapabilities.currentExtent.height;
        if (surfaceCapabilities.supportedUsageFlags & VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) 
            std::cout << "\nSurface supported usage: VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT";

        // PFN_vkGetPhysicalDeviceSurfaceSupportKHR
        // PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR
        // PFN_vkGetPhysicalDeviceSurfaceFormatsKHR
        // PFN_vkGetPhysicalDeviceSurfacePresentModesKHR

        // VkSwapchainCreateInfoKHR cSwapchainInfo{
        //     .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        //     .surface = context.surface,
        //     // uint32_t                         minImageCount;
        //     // VkFormat                         imageFormat;
        //     // VkColorSpaceKHR                  imageColorSpace;
        //     // VkExtent2D                       imageExtent;
        //     // uint32_t                         imageArrayLayers;
        //     .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        //     .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,                  //ak si dobre pametam malo by to znamenat ze exluzivne len jedna qFamily alebo q bude robit so swapchainou
        //     // uint32_t                         queueFamilyIndexCount;
        //     // const uint32_t*                  pQueueFamilyIndices;
        //     // VkSurfaceTransformFlagBitsKHR    preTransform;
        //     .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        //     // VkPresentModeKHR                 presentMode;
        //     // VkBool32                         clipped;
        //     // VkSwapchainKHR                   oldSwapchain;
        // };

        //VK_CHECK(d.vkCreateSwapchainKHR(context.device, &cSwapchainInfo, VK_NULL_HANDLE, &context.swapchain))
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
  

    if (context.device)
    {
        d.vkDestroyDevice(context.device, VK_NULL_HANDLE);
    }
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