#pragma once

#include <qcoreapplication.h>
#include <qstringview.h>
#ifndef RETO_SCENEWIDGET_2D_HPP
#define RETO_SCENEWIDGET_2D_HPP

#include "Core/AbstractClasses/SceneWidget/AbstractSceneWidget.hpp"
#include <QAbstractNativeEventFilter>
#include <QWidget>
#include <qtmetamacros.h>
#include <vector>

namespace Retoccilus::Editor::SceneWidget {
    class RSceneWidget2D : public Core::AbstractClasses::SceneWidget::RAbstractSceneWidget {
        Q_OBJECT
    public:
        explicit RSceneWidget2D(QWidget *parent = nullptr);
        ~RSceneWidget2D();

    protected:
        void initRenderPipeline() override;
        void render() override;
        void initVulkan() override;
        void cleanup() override;

    private:
        void cleanupSwapChain();
        void createFramebuffers();
        void createSyncObjects();
        void createImageViews();
        void createCommandPool();
        void createCommandBuffer();
        void createRenderPass();
        void createPipelineLayout();
        void createGraphicsPipeline();
        VkShaderModule createShaderModule(const std::vector<char>& code);

    private:
        VkRenderPass m_renderPass = VK_NULL_HANDLE;
        VkPipeline m_pipeline = VK_NULL_HANDLE;
        VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
        VkCommandPool m_commandPool = VK_NULL_HANDLE;
        VkCommandBuffer m_commandBuffer = VK_NULL_HANDLE;
        std::vector<VkFramebuffer> m_swapChainFramebuffers;
        std::vector<VkImageView> m_swapChainImageViews;

        VkSemaphore m_imageAvailableSemaphore = VK_NULL_HANDLE;
        VkSemaphore m_renderFinishedSemaphore = VK_NULL_HANDLE;
        VkFence m_inFlightFence = VK_NULL_HANDLE;
    };
} // namespace Retoccilus::Editor::SceneWidget
#endif // RETO_SCENEWIDGET_2D_HPP
