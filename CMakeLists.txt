cmake_minimum_required(VERSION 3.30.0)


project(Vulkan2 VERSION 1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)


# Collect source files
set(SOURCES
    Vulkan2_projekt/src/main.cpp
    src/dynamic.cpp
)
#file(GLOB SOURCES CONFIGUTE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2 projekt/src/*.cpp}")

# definuje executable
add_executable(${PROJECT_NAME} ${SOURCES})


target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Vulkan2_projekt/include)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(${PROJECT_NAME} PRIVATE RELEASE)
else()
    message(WARNING "Unknown build type ${CMAKE_BUILD_TYPE}")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE VK_NO_PROTOTYPES)
if(WIN32)
	target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_WIN32_KHR)
elseif(UNIX)
	target_compile_definitions(${PROJECT_NAME} PRIVATE VK_USE_PLATFORM_XLIB_KHR)
endif()



find_package(SDL3 CONFIG REQUIRED)
if(WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
elseif(UNIX)
	target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 X11)
endif()

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
