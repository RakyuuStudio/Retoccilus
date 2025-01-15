#include "Core/VulkanWrapper/VulkanWrapper.h"
using namespace Retoccilus::Core;
#include <iostream>

int main() {
    try {
        VulkanWrapper app(800, 600, "Vulkan Window");

        app.initVulkan();
        app.mainLoop();
        app.cleanup();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
