#pragma once

#include "vulkan/vulkan.h"


struct Dynamic
{
    private:
    // Pointer na nacitanu kniznicu. 
    // HMODULE alebo HINSTANCE je to to iste.
    HMODULE lib = nullptr;

    public:
    void loadLib(const char* path);
    void freeLib();
};