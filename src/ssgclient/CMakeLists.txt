#include_directories(${Boost_INCLUDE_DIRS})

add_executable(SSGClient main.cpp)

add_dependencies(SSGClient SSG)
target_include_directories(SSGClient PUBLIC ${Boost_INCLUDE_DIRS})
IF (WIN32)
    target_link_libraries(SSGClient ${Boost_LIBRARIES} ${Vulkan_LIBRARIES} ws2_32 glfw3.a)
ELSE()
    target_link_libraries(SSGClient ${Boost_LIBRARIES} ${Vulkan_LIBRARIES})
ENDIF()