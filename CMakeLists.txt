### SpaceStationGalaxy Master CMake
cmake_minimum_required(VERSION 3.7.1)
project(ssg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../../bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../../lib) # TODO: Not working???

## Platform-Specific Logic
#################################################
## TODO: Move these to Env Variables
#################################################
unset(VULKAN_SDK CACHE)
unset(BOOST_ROOT CACHE)
set(BOOST_ROOT E:/SDK/CPP/boost_1_64_0)
set(VULKAN_SDK E:/SDK/CPP/Vulkan/1.0.46.0)
#################################################

## FindPackage Flags
#################################################
#set(Boost_DEBUG 1)
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
#################################################

## Resolve External Dependencies
#################################################
### Boost
MESSAGE(STATUS)
MESSAGE(STATUS "*******************************")
MESSAGE(STATUS "BOOST")
MESSAGE(STATUS "*******************************")
unset(Boost_FOUND CACHE)
unset(Boost_INCLUDE_DIR CACHE)
unset(Boost_LIBRARIES CACHE)
find_package(Boost
        1.64.0
       COMPONENTS atomic container context
            coroutine date_time exception fiber
            filesystem graph iostreams locale
            log_setup log math_c99f math_c99l
            math_c99 math_tr1f math_tr1l math_tr1
            prg_exec_monitor program_options random regex
            serialization signals system test_exec_monitor
            thread timer type_erasure unit_test_framework
            wave wserialization chrono
        REQUIRED)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
    add_definitions("-DHAS_BOOST")

    MESSAGE(STATUS "Boost Version: " ${Boost_VERSION})
    MESSAGE(STATUS "Boost Include: " ${Boost_INCLUDE_DIR})
    MESSAGE(STATUS "Boost Libraries: " ${Boost_LIBRARIES})
    IF (Boost_SYSTEM_FOUND)
        add_definitions("-DHAS_BOOST_SYSTEM")
    ENDIF()
    IF (Boost_THREAD_FOUND)
        add_definitions("-DHAS_BOOST_THREAD")
    ENDIF()
    IF (Boost_LOG_FOUND)
        add_definitions("-DHAS_BOOST_LOG")
    ENDIF()
ENDIF()

### Doxygen
MESSAGE(STATUS)
MESSAGE(STATUS "*******************************")
MESSAGE(STATUS "DOXYGEN")
MESSAGE(STATUS "*******************************")
unset(Doxygen_FOUND CACHE)
unset(DOXYGEN_VERSION CACHE)
unset(DOXYGEN_EXECUTABLE CACHE)
find_package(Doxygen 1.8.13 REQUIRED)
IF (Doxygen_FOUND)
    add_definitions("-DHAS_DOXYGEN")
    MESSAGE(STATUS "Doxygen Version: " ${DOXYGEN_VERSION})
    MESSAGE(STATUS "Doxygen Executable: " ${DOXYGEN_EXECUTABLE})
ENDIF()

### Vulkan
MESSAGE(STATUS)
MESSAGE(STATUS "*******************************")
MESSAGE(STATUS "VULKAN")
MESSAGE(STATUS "*******************************")
MESSAGE(STATUS "Is Windows?: " ${WIN32})
MESSAGE(STATUS "Void Ptr Size: " ${CMAKE_SIZEOF_VOID_P})
MESSAGE(STATUS "SDK Location: " ${VULKAN_SDK})
unset(Vulkan_FOUND CACHE)
unset(Vulkan_INCLUDE_DIR CACHE)
unset(Vulkan_LIBRARIES CACHE)
unset(Vulkan_STATIC_LIBRARIES CACHE)
unset(Vulkan_VERSION CACHE)
#find_package(Vulkan 1.0.46.0 REQUIRED) # Getting vulkan-1.dll back, need vulkan-1.lib (or vulkan.a?)
IF (WIN32)
    find_path(Vulkan_INCLUDE_DIR NAMES vulkan/vulkan.h PATHS ${VULKAN_SDK}/Include NO_DEFAULT_PATH)
    find_file(Vulkan_LIBRARIES NAMES vulkan-1.lib vulkan-1.a libvulkan-1.a PATHS ${VULKAN_SDK}/Lib NO_DEFAULT_PATH)
    find_file(Vulkan_STATIC_LIBRARIES NAMES vkstatic.1.lib vkstatic.1.a libvkstatic.1.a PATHS ${VULKAN_SDK}/Lib NO_DEFAULT_PATH)
ELSE()
    find_path(Vulkan_INCLUDE_DIR NAMES vulkan/vulkan.h PATHS ${VULKAN_SDK}/Include NO_DEFAULT_PATH)
    find_file(Vulkan_LIBRARIES NAMES vulkan.lib vulkan.a libvulkan.a PATHS ${VULKAN_SDK}/Lib NO_DEFAULT_PATH)
ENDIF()

IF (Vulkan_LIBRARIES)
    add_definitions("-DHAS_VULKAN")
    include_directories(${Vulkan_INCLUDE_DIR})

    MESSAGE(STATUS)
    MESSAGE(STATUS "Vulkan Include: " ${Vulkan_INCLUDE_DIR})
    MESSAGE(STATUS "Vulkan Libraries: " ${Vulkan_LIBRARIES})
    MESSAGE(STATUS "Vulkan Static Libraries: " ${Vulkan_STATIC_LIBRARIES})
ENDIF()

### GLFW - TODO
unset(glfw_FOUND CACHE)
add_definitions("-DHAS_GLFW")
include_directories(E:/SDK/CPP/glfw/include)
link_directories(E:/SDK/CPP/glfw/lib)
#find_package(glfw)

### GLM - TODO
unset(glm_FOUND CACHE)
add_definitions("-DHAS_GLM")
include_directories(E:/SDK/CPP/glm)
#find_package(glm)
#################################################

## Add Projects
add_subdirectory(src/ssg)
add_subdirectory(src/ssgclient)
add_subdirectory(src/ssgserver)