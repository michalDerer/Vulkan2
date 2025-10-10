cmake_minimum_required(VERSION 4.0.0)

project(Vulkan2 VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)


# Add include directory
#include_directories(include)
#include_directories(${CMAKE_SOURCE_DIR}/include))

# Collect source files
set(SOURCES
    src/include/dynamic.hpp
    src/include/macross.hpp
    src/main.cpp
    src/dynamic.cpp
)


# Define executable
add_executable(${PROJECT_NAME} ${SOURCES})


target_compile_definitions(${PROJECT_NAME} PRIVATE
    VK_NO_PROTOTYPES
    VK_USE_PLATFORM_WIN32_KHR
)


find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

 find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
# Or use the header-only version
#find_package(glm CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm-header-only)

# Nechcem pouzivat Vulkan SDK
#find_package(Vulkan REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)
#Chcem pouzit Vulkan poskitnuty cez vcpkg 
find_package(VulkanHeaders CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}  PRIVATE Vulkan::Headers)