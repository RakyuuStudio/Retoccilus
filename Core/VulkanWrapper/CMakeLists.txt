add_library(VulkanWrapper STATIC
    VulkanWrapper.cpp
    VulkanWrapper.h
)

target_link_libraries(VulkanWrapper PUBLIC Vulkan::Vulkan glfw glm)