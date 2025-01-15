#ifndef VULKAN_WRAPPER_H
#define VULKAN_WRAPPER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <optional>
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace Retoccilus::Core {

    class AbstractRenderLayer {
      public:
        virtual ~AbstractRenderLayer() = default;
        virtual void initialize() = 0;
        virtual void render() = 0;
        virtual void cleanup() = 0;
    };

    class VulkanWrapper : public AbstractRenderLayer {
      public:
        struct QueueFamilyIndices {
            std::optional<uint32_t> graphicsFamily;
            std::optional<uint32_t> presentFamily;

            bool isComplete() {
                return graphicsFamily.has_value() && presentFamily.has_value();
            }
        };

        VulkanWrapper(uint32_t width, uint32_t height, const std::string &title);
        ~VulkanWrapper();

        void initialize() override { initVulkan(); }
        void render() override { mainLoop(); }
        void cleanup() override { AbstractRenderLayer::cleanup(); }

        void initVulkan();
        void mainLoop();

        // Configuration methods
        void setDeviceExtensions(const std::vector<const char *> &extensions);
        void setValidationLayers(const std::vector<const char *> &layers);
        void setSwapchainImageCount(uint32_t count);
        void setMaxFramesInFlight(uint32_t count);

      private:
        void createInstance();
        void setupDebugMessenger();
        void createSurface();
        void pickPhysicalDevice();
        void createLogicalDevice();
        void createSwapChain();
        void createImageViews();
        void createSyncObjects();

        // Helper functions
        bool                      checkValidationLayerSupport();
        std::vector<const char *> getRequiredExtensions();
        bool                      isDeviceSuitable(VkPhysicalDevice device);
        QueueFamilyIndices        findQueueFamilies(VkPhysicalDevice device);
        bool                      checkDeviceExtensionSupport(VkPhysicalDevice device);
        VkSurfaceFormatKHR        chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats);
        VkPresentModeKHR          chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes);
        VkExtent2D                chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

        // Window related
        GLFWwindow *window;
        uint32_t    windowWidth;
        uint32_t    windowHeight;

        // Vulkan objects
        VkInstance               instance;
        VkDebugUtilsMessengerEXT debugMessenger;
        VkSurfaceKHR             surface;
        VkPhysicalDevice         physicalDevice = VK_NULL_HANDLE;
        VkDevice                 device;
        VkQueue                  graphicsQueue;
        VkQueue                  presentQueue;
        VkSwapchainKHR           swapChain;
        std::vector<VkImage>     swapChainImages;
        VkFormat                 swapChainImageFormat;
        VkExtent2D               swapChainExtent;
        std::vector<VkImageView> swapChainImageViews;

        // Synchronization
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence>     inFlightFences;
        size_t                   currentFrame = 0;

        // Configuration
        std::vector<const char *> deviceExtensions;
        std::vector<const char *> validationLayers;
        uint32_t                  swapchainImageCount = 2;
        uint32_t                  maxFramesInFlight = 2;

        // Debug
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT             messageType,
            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
            void                                       *pUserData);
    };

} // namespace Retoccilus::Core

#endif // VULKAN_WRAPPER_H
