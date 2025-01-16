cmake_minimum_required(VERSION 3.10)
project(Retoccilus)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)
# Add VulkanWrapper library

include_directories(ThirdPartyLibraries/stb)

add_subdirectory(Core/VulkanWrapper)
add_subdirectory(Core/SceneWidget)
add_subdirectory(ThirdPartyLibraries/glfw)
add_subdirectory(ThirdPartyLibraries/glm)
# Add executable
add_executable(Retoccilus main.cpp)

# Link libraries
target_link_libraries(Retoccilus PRIVATE Vulkan::Vulkan glfw glm)
target_link_libraries(Retoccilus PRIVATE VulkanWrapper Rt2DSceneWidget)