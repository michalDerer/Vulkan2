cmake_minimum_required(VERSION 3.30.0)


project(Vulkan2 VERSION 1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)


#Collect source files
#set(
#   SOURCES
#   Vulkan2_projekt/src/main.cpp
#   Vulkan2_projekt/src/dynamic.cpp
#)
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Vulkan2_projekt/src/*.cpp")		#pre rekurzivne prehldanie do hlbky pouzi GLOB_RECURSE
#message(STATUS "Source files: ${SOURCES}")


# definuje projekt vytvarajuci executable
add_executable(${PROJECT_NAME} ${SOURCES})


#musi ist po add_executable,... ktory vytvori target
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/Vulkan2_projekt/include)
#musi ist po add_executable,... ktory vytvori target
#target_link_libraries()



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



if (WIN32)
    message(STATUS "Configuring for Windows...")

    find_package(SDL3 CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)
    #find_package(Vulkan REQUIRED)			#toto najde externe nainstalovane Vulkan SDK
    find_package(VulkanHeaders CONFIG REQUIRED)		#toto najde Vulkan SDK headers z vcpkg

    target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE
        SDL3::SDL3
	#X11				
	glm::glm			#glm
	#glm::glm-header-only 		#glm header only varianta
        #Vulkan::Vulkan			#Vulkan library z externeho Vulkan SDK
	Vulkan::Headers			#Vulkan header library z vcpkg
    )

elseif (UNIX)
    message(STATUS "Configuring for Linux...")

    find_package(SDL3 CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)
    #find_package(Vulkan REQUIRED)			#toto najde externe nainstalovane Vulkan SDK
    find_package(VulkanHeaders CONFIG REQUIRED)		#toto najde Vulkan SDK headers z vcpkg

    target_link_libraries(
	${PROJECT_NAME} 
        PRIVATE 
        SDL3::SDL3 
        X11				#linux only lib pre komunikaciu s oknom
        glm::glm			#glm
	#glm::glm-header-only		#glm header only varianta
	#Vulkan::Vulkan			#Vulkan library z externeho Vulkan SDK
	Vulkan::Headers			#Vulkan header library z vcpkg
    )

endif()






