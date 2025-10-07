cmake_minimum_required(VERSION 4.0.0)

project(Vulkanik VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

# Add include directory
#include_directories(include)
#include_directories(${CMAKE_SOURCE_DIR}/include))


# Collect source files
set(SOURCES
    src/main.cpp
    #src/math.cpp
)

# Define executable
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SDL3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

 find_package(glm CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
# Or use the header-only version
#find_package(glm CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm-header-only)

find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)