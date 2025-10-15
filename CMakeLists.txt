cmake_minimum_required(VERSION 3.30.0)

project(Vulkan2 VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)


# Add include directory
#include_directories(include)
#include_directories(${CMAKE_SOURCE_DIR}/include))

# Collect source files
set(SOURCES
    src/include/dynamic.hpp
    src/include/macros.hpp
    src/main.cpp
    src/dynamic.cpp
)


# definuje executable
add_executable(${PROJECT_NAME} ${SOURCES})



target_compile_definitions(${PROJECT_NAME} PRIVATE VK_NO_PROTOTYPES)
if(WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
elseif(UNIX)
	target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_XLIB_KHR)
endif()



#${CMAKE_CURRENT_SOURCE_DIR}
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/include)


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
