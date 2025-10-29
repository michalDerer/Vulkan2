
#pragma once

#ifndef DEBUG
#define VK_CHECK(value)  
#else
#define VK_CHECK(value)                                                                                     \
do {                                                                                                        \
    VkResult vValue = value;                                                                                \
    if (vValue != VK_SUCCESS)                                                                               \
    {                                                                                                       \
        /*fprintf(stderr, "Vulkan error: %d at %s:%d\n", vValue, __FILE__, __LINE__);*/                     \
        std::cerr << "Vulkan error: " << vValue << " at " << __FILE__ << ":" << __LINE__ << std::endl;      \
        abort();                                                                                            \
    }                                                                                                       \
} while (0);                                                                                       
#endif

#ifndef DEBUG
#define NULL_CHECK(p)  
#else
#define NULL_CHECK(p)                                                                                       \
do {                                                                                                        \
    void* pP = (void*)p;                                                                                    \
    if (pP == NULL)                                                                                         \
    {                                                                                                       \
        std::cerr << "Null error at " << __FILE__ << ":" << __LINE__ << std::endl;                          \
        abort();                                                                                            \
    }                                                                                                       \
} while (0);
#endif