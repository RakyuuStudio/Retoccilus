#include "SceneWidget2D.hpp"
#include "Core/FileOperation/FileOperation.hpp"
#include <QFile>
#include <QVulkanInstance>
#include <QAbstractNativeEventFilter>
#include <QEvent>
#include <QMessageBox>
#include <qnamespace.h>
#include <QPaintDevice>
#include <QWidget>
#include <stdexcept>
#include <vulkan/vulkan_core.h>
#include <QDir>
#include <QTimer>

// #include <fstream>

namespace Retoccilus::Editor::SceneWidget {
    RSceneWidget2D::RSceneWidget2D(QWidget *parent)
        : Core::AbstractClasses::SceneWidget::RAbstractSceneWidget(parent) {
        // 等待widget完全显示后再初始化Vulkan
        QTimer::singleShot(0, this, [this]() {
            try {
                initVulkan();
                initRenderPipeline();
            } catch (const std::exception &e) {
                qFatal("Vulkan initialization failed: %s", e.what());
            }
        });
    }

    RSceneWidget2D::~RSceneWidget2D() {
        if (device() != VK_NULL_HANDLE) {
            vkDeviceWaitIdle(device());

            cleanupSwapChain();

            vkDestroyPipeline(device(), m_pipeline, nullptr);
            vkDestroyPipelineLayout(device(), m_pipelineLayout, nullptr);
            vkDestroyRenderPass(device(), m_renderPass, nullptr);

            vkDestroySemaphore(device(), m_imageAvailableSemaphore, nullptr);
            vkDestroySemaphore(device(), m_renderFinishedSemaphore, nullptr);
            vkDestroyFence(device(), m_inFlightFence, nullptr);

            vkDestroyCommandPool(device(), m_commandPool, nullptr);
        }
    }

    void RSceneWidget2D::initVulkan() {
        this->createInstance();
        this->createSurface();
        this->setupDevice();
        this->createSwapChain();
    }

    void RSceneWidget2D::cleanupSwapChain() {
        // 先清理帧缓冲
        for (auto framebuffer : m_swapChainFramebuffers) {
            vkDestroyFramebuffer(device(), framebuffer, nullptr);
        }
        m_swapChainFramebuffers.clear();

        // 再清理图像视图
        for (auto imageView : m_swapChainImageViews) {
            vkDestroyImageView(device(), imageView, nullptr);
        }
        m_swapChainImageViews.clear();
    }

    void RSceneWidget2D::initRenderPipeline() {
        createCommandPool();
        createCommandBuffer();
        createImageViews();
        createRenderPass();
        createFramebuffers();
        createSyncObjects();
        createPipelineLayout();
        createGraphicsPipeline();
    }

    VkShaderModule RSceneWidget2D::createShaderModule(const std::vector<char> &code) {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode    = reinterpret_cast<const uint32_t *>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create shader module!");
        }

        return shaderModule;
    }

    void RSceneWidget2D::createFramebuffers() {
        m_swapChainFramebuffers.resize(swapChainImages().size());

        for (size_t i = 0; i < swapChainImages().size(); i++) {
            VkImageView attachment = m_swapChainImageViews[i];

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass      = m_renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments    = &attachment;
            framebufferInfo.width           = swapChainExtent().width;
            framebufferInfo.height          = swapChainExtent().height;
            framebufferInfo.layers          = 1;

            if (vkCreateFramebuffer(device(), &framebufferInfo, nullptr,
                                    &m_swapChainFramebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("Failed to create framebuffer!");
            }
        }
    }

    // void RSceneWidget2D::createInstance() {
    //     VkApplicationInfo appInfo{};
    //     appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    //     appInfo.pApplicationName   = "Scene Widget 2D";
    //     appInfo.applicationVersion = VK_MAKE_VERSION(1, 3, 0);
    //     appInfo.pEngineName        = "No Engine";
    //     appInfo.engineVersion      = VK_MAKE_VERSION(1, 3, 0);
    //     appInfo.apiVersion         = VK_API_VERSION_1_3;

    //     VkInstanceCreateInfo createInfo{};
    //     createInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    //     createInfo.pApplicationInfo = &appInfo;

    //     // TODO: Instance Extension and Validation Layers

    //     vkCreateInstance(&createInfo, nullptr, *instance);
    // }

    void RSceneWidget2D::createCommandPool() {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = 0; // 使用图形队列族

        if (vkCreateCommandPool(device(), &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create command pool!");
        }
    }

    void RSceneWidget2D::createCommandBuffer() {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool        = m_commandPool;
        allocInfo.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(device(), &allocInfo, &m_commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("Failed to allocate command buffer!");
        }
    }

    void RSceneWidget2D::createRenderPass() {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format         = swapChainFormat();
        colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments    = &colorAttachmentRef;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType           = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments    = &colorAttachment;
        renderPassInfo.subpassCount    = 1;
        renderPassInfo.pSubpasses      = &subpass;

        if (vkCreateRenderPass(device(), &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create render pass!");
        }
    }

    void RSceneWidget2D::createPipelineLayout() {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

        if (vkCreatePipelineLayout(device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) !=
            VK_SUCCESS) {
            throw std::runtime_error("Failed to create pipeline layout!");
        }
    }

    void RSceneWidget2D::createGraphicsPipeline() {
        Core::FileOperation::FileOperation fo;

        // FIXME: This MIGHT be cause of error.
        // QFile vertFile("E:\\Retoccilus\\Resources\\Shaders\\vert.spv");
        // QFile fragFile("E:\\Retoccilus\\Resources\\Shaders\\frag.spv");

        qDebug() << "Query Vertex Shader in:" << QDir::currentPath() + "/shaders/vert.spv";
        qDebug() << "Query Fragment Shader in:" << QDir::currentPath() + "/shaders/frag.spv";

        QFile vertFile(QString(QDir::currentPath() + "/shaders/vert.spv"));
        QFile fragFile(QString(QDir::currentPath() + "/shaders/frag.spv"));

        if (!vertFile.open(QIODevice::ReadOnly) || !fragFile.open(QIODevice::ReadOnly)) {
            throw std::runtime_error("Failed to open shader files");
        }

        QByteArray vertData = vertFile.readAll();
        QByteArray fragData = fragFile.readAll();

        std::vector<char> vertShaderCode(vertData.begin(), vertData.end());
        std::vector<char> fragShaderCode(fragData.begin(), fragData.end());

        vertFile.close();
        fragFile.close();

        if (vertShaderCode.empty()) {
            qDebug() << "Cannot read shader code!";
        }

        qDebug() << "Shader [Vertex]: " << vertShaderCode.data();
        qDebug() << "Shader [Fragment]: " << fragShaderCode.data();

        VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
        VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage  = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName  = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName  = "main";

        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {vertShaderStageInfo,
                                                                       fragShaderStageInfo};

        // 配置管线的其他部分...
        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType      = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
        pipelineInfo.pStages    = shaderStages.data();
        pipelineInfo.layout     = m_pipelineLayout;
        pipelineInfo.renderPass = m_renderPass;

        // 添加顶点输入状态
        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

        // 添加输入装配状态
        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        // 添加视口和裁剪状态
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount  = 1;

        // 添加光栅状态
        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType       = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth   = 1.0f;
        rasterizer.cullMode    = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace   = VK_FRONT_FACE_CLOCKWISE;

        // 添加多重采样状态
        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        // 添加颜色混合状态
        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                              VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType           = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments    = &colorBlendAttachment;

        // 添加动态状态
        std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
                                                     VK_DYNAMIC_STATE_SCISSOR};
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        dynamicState.pDynamicStates    = dynamicStates.data();

        // 更新管线创建信息
        pipelineInfo.pVertexInputState   = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState      = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState   = &multisampling;
        pipelineInfo.pColorBlendState    = &colorBlending;
        pipelineInfo.pDynamicState       = &dynamicState;

        if (vkCreateGraphicsPipelines(device(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr,
                                      &m_pipeline) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create graphics pipeline!");
        }

        vkDestroyShaderModule(device(), fragShaderModule, nullptr);
        vkDestroyShaderModule(device(), vertShaderModule, nullptr);
    }

    void RSceneWidget2D::render() {
        if (device() == VK_NULL_HANDLE || m_inFlightFence == VK_NULL_HANDLE) {
            qDebug() << "Device or fence is invalid";
            return;
        }

        vkWaitForFences(device(), 1, &m_inFlightFence, VK_TRUE, UINT64_MAX);
        vkResetFences(device(), 1, &m_inFlightFence);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(device(), swapChain(), UINT64_MAX, m_imageAvailableSemaphore,
                              VK_NULL_HANDLE, &imageIndex);

        vkResetCommandBuffer(m_commandBuffer, 0);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        vkBeginCommandBuffer(m_commandBuffer, &beginInfo);

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass        = m_renderPass;
        renderPassInfo.framebuffer       = m_swapChainFramebuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChainExtent();

        VkClearValue clearColor        = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues    = &clearColor;

        vkCmdBeginRenderPass(m_commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
        vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline);
        vkCmdDraw(m_commandBuffer, 3, 1, 0, 0);
        vkCmdEndRenderPass(m_commandBuffer);

        vkEndCommandBuffer(m_commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[]      = {m_imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount     = 1;
        submitInfo.pWaitSemaphores        = waitSemaphores;
        submitInfo.pWaitDstStageMask      = waitStages;
        submitInfo.commandBufferCount     = 1;
        submitInfo.pCommandBuffers        = &m_commandBuffer;

        VkSemaphore signalSemaphores[]  = {m_renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores    = signalSemaphores;

        vkQueueSubmit(graphicsQueue(), 1, &submitInfo, m_inFlightFence);

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores    = signalSemaphores;

        VkSwapchainKHR swapChainHandle = swapChain();
        presentInfo.swapchainCount     = 1;
        presentInfo.pSwapchains        = &swapChainHandle;
        presentInfo.pImageIndices      = &imageIndex;

        vkQueuePresentKHR(presentQueue(), &presentInfo);
    }

    void RSceneWidget2D::createSyncObjects() {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (device() == VK_NULL_HANDLE) {
            throw std::runtime_error("Cannot create sync objects: device is null");
        }

        if (vkCreateSemaphore(device(), &semaphoreInfo, nullptr, &m_imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(device(), &semaphoreInfo, nullptr, &m_renderFinishedSemaphore) != VK_SUCCESS ||
            vkCreateFence(device(), &fenceInfo, nullptr, &m_inFlightFence) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create synchronization objects!");
        }

        qDebug() << "Sync objects created successfully";
    }

    void RSceneWidget2D::createImageViews() {
        m_swapChainImageViews.resize(swapChainImages().size());

        for (size_t i = 0; i < swapChainImages().size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image    = swapChainImages()[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format   = swapChainFormat();

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel   = 0;
            createInfo.subresourceRange.levelCount     = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount     = 1;

            if (vkCreateImageView(device(), &createInfo, nullptr, &m_swapChainImageViews[i]) !=
                VK_SUCCESS) {
                throw std::runtime_error("Failed to create image views!");
            }
        }
    }

    void RSceneWidget2D::cleanup() {
        if (device() != VK_NULL_HANDLE) {
            vkDeviceWaitIdle(device());

            // 先清理同步对象
            if (m_inFlightFence != VK_NULL_HANDLE) {
                vkDestroyFence(device(), m_inFlightFence, nullptr);
                m_inFlightFence = VK_NULL_HANDLE;
            }
            if (m_imageAvailableSemaphore != VK_NULL_HANDLE) {
                vkDestroySemaphore(device(), m_imageAvailableSemaphore, nullptr);
                m_imageAvailableSemaphore = VK_NULL_HANDLE;
            }
            if (m_renderFinishedSemaphore != VK_NULL_HANDLE) {
                vkDestroySemaphore(device(), m_renderFinishedSemaphore, nullptr);
                m_renderFinishedSemaphore = VK_NULL_HANDLE;
            }

            cleanupSwapChain();
            
            if (m_pipeline != VK_NULL_HANDLE) {
                vkDestroyPipeline(device(), m_pipeline, nullptr);
                m_pipeline = VK_NULL_HANDLE;
            }
            if (m_pipelineLayout != VK_NULL_HANDLE) {
                vkDestroyPipelineLayout(device(), m_pipelineLayout, nullptr);
                m_pipelineLayout = VK_NULL_HANDLE;
            }
            if (m_renderPass != VK_NULL_HANDLE) {
                vkDestroyRenderPass(device(), m_renderPass, nullptr);
                m_renderPass = VK_NULL_HANDLE;
            }
            if (m_commandPool != VK_NULL_HANDLE) {
                vkDestroyCommandPool(device(), m_commandPool, nullptr);
                m_commandPool = VK_NULL_HANDLE;
            }
        }

        // 调用基类的cleanup
        RAbstractSceneWidget::cleanup();
    }
} // namespace Retoccilus::Editor::SceneWidget