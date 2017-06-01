add_library(SSG STATIC plugin.cpp)

target_link_libraries(SSG ${Vulkan_LIBS})