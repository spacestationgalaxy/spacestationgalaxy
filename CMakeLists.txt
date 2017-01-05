### SpaceStationGalaxy Master CMake
cmake_minimum_required(VERSION 3.7.1)
project(ssg)

## Platform-Specific Logic

## Resolve External Dependencies
#find_package(Boost REQUIRED iostream thread)
#find_package(Vulkan)
#find_package(glfw)
#find_package(glm)

## Resolve Internal Dependencies

## Add Projects
add_subdirectory(src/ssg)
add_subdirectory(src/ssgclient)
add_subdirectory(src/ssgserver)