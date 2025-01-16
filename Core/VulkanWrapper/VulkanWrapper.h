#ifndef VULKAN_WRAPPER_H
#define VULKAN_WRAPPER_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>
#include <array>  // 为Vertex结构体的getAttributeDescriptions()添加

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
        void cleanup() override;

        void initVulkan();
        void mainLoop();

        // Configuration methods
        void setDeviceExtensions(const std::vector<const char *> &extensions);
        void setValidationLayers(const std::vector<const char *> &layers);
        void setSwapchainImageCount(uint32_t count);
        void setMaxFramesInFlight(uint32_t count);

        // 添加新的接口
        bool windowShouldClose() const { return glfwWindowShouldClose(window); }
        void resizeWindow(uint32_t width, uint32_t height);

        // 渲染相关
        void setTransformMatrix(const glm::mat4 &transform);
        void loadTexture(const std::string &path, uint32_t &textureId);
        void unloadTexture(uint32_t textureId);

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

        // 添加新的成员
        struct TextureData {
            VkImage        image;
            VkDeviceMemory memory;
            VkImageView    view;
            VkSampler      sampler;
        };
        std::unordered_map<uint32_t, TextureData> textures;
        uint32_t                                  nextTextureId = 1;

        // 纹理相关辅助函数
        void createImage(uint32_t width, uint32_t height, VkFormat format,
                        VkImageTiling tiling, VkImageUsageFlags usage,
                        VkMemoryPropertyFlags properties, VkImage& image,
                        VkDeviceMemory& imageMemory);
                        
        void transitionImageLayout(VkImage image, VkFormat format,
                                  VkImageLayout oldLayout, VkImageLayout newLayout);
                                  
        void copyBufferToImage(VkBuffer buffer, VkImage image,
                              uint32_t width, uint32_t height);
                              
        VkImageView createImageView(VkImage image, VkFormat format);
        
        VkSampler createTextureSampler();
        
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                         VkMemoryPropertyFlags properties, VkBuffer& buffer,
                         VkDeviceMemory& bufferMemory);
                         
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);

        // 添加新的辅助函数
        void createTextureImage(const std::string& path, VkImage& image, 
                              VkDeviceMemory& imageMemory, uint32_t& width, uint32_t& height);
        void createTextureImageView(VkImage image, VkImageView& imageView);
        void createTextureSamplerIfNeeded();
        
        // 添加新的成员
        VkCommandPool commandPool;  // 添加命令池
        VkSampler textureSampler = VK_NULL_HANDLE;  // 共享的纹理采样器

        void createCommandPool();

        // 渲染管线相关
        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;
        std::vector<VkFramebuffer> swapChainFramebuffers;

        // 渲染管线相关函数
        void createRenderPass();
        void createGraphicsPipeline();
        void createFramebuffers();
        
        // 着色器相关函数
        VkShaderModule createShaderModule(const std::vector<char>& code);
        static std::vector<char> readFile(const std::string& filename);

        // 顶点相关
        struct Vertex {
            glm::vec2 pos;
            glm::vec2 texCoord;

            static VkVertexInputBindingDescription getBindingDescription() {
                VkVertexInputBindingDescription bindingDescription{};
                bindingDescription.binding = 0;
                bindingDescription.stride = sizeof(Vertex);
                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
                return bindingDescription;
            }

            static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
                std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

                attributeDescriptions[0].binding = 0;
                attributeDescriptions[0].location = 0;
                attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[0].offset = offsetof(Vertex, pos);

                attributeDescriptions[1].binding = 0;
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[1].offset = offsetof(Vertex, texCoord);

                return attributeDescriptions;
            }
        };

        // Uniform Buffer相关
        struct UniformBufferObject {
            alignas(16) glm::mat4 model;
            alignas(16) glm::mat4 view;
            alignas(16) glm::mat4 proj;
        };

        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        std::vector<void*> uniformBuffersMapped;

        // Uniform Buffer相关函数
        void createUniformBuffers();
        void updateUniformBuffer(uint32_t currentImage, const glm::mat4& modelMatrix);
        void destroyUniformBuffers();

        // 相机矩阵
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::mat4 projMatrix = glm::mat4(1.0f);

        // 描述符相关
        VkDescriptorSetLayout descriptorSetLayout;
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;

        // 描述符相关函数
        void createDescriptorSetLayout();
        void createDescriptorPool();
        void createDescriptorSets();

        // 添加新的函数声明
        void updateDescriptorSet(uint32_t currentImage, VkImageView textureImageView);

        // 顶点和索引缓冲区相关
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        std::vector<VkCommandBuffer> commandBuffers;

        // 渲染相关函数
        void createVertexBuffer();
        void createIndexBuffer();
        void createCommandBuffers();
        void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

        // 默认的矩形顶点数据
        const std::vector<Vertex> vertices = {
            {{-0.5f, -0.5f}, {0.0f, 0.0f}},
            {{ 0.5f, -0.5f}, {1.0f, 0.0f}},
            {{ 0.5f,  0.5f}, {1.0f, 1.0f}},
            {{-0.5f,  0.5f}, {0.0f, 1.0f}}
        };

        const std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0
        };

        // 交换链重建相关函数
        void recreateSwapChain();
        void cleanupSwapChain();
        void createSwapChainResources();
    };

} // namespace Retoccilus::Core

#endif // VULKAN_WRAPPER_H
