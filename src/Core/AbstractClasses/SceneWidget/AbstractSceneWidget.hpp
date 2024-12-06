#ifndef ABSTRACT_SCENE_WIDGET_H
#define ABSTRACT_SCENE_WIDGET_H
#pragma once

#include <GLFW/glfw3.h>
#include <QWidget>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <vulkan/vulkan.h>
#include <set>
#include <QDebug>

namespace Retoccilus::Core::AbstractClasses::SceneWidget {
    class RAbstractSceneWidget : public QWidget {
        Q_OBJECT
    public:
        explicit RAbstractSceneWidget(QWidget *parent = nullptr);
        virtual ~RAbstractSceneWidget();

    protected:
        VkInstance instance() const {
            return m_instance;
        }
        VkDevice device() const {
            return m_device;
        }
        VkPhysicalDevice physicalDevice() const {
            return m_physicalDevice;
        }
        VkSwapchainKHR swapChain() const {
            return m_swapChain;
        }
        VkFormat swapChainFormat() const {
            return m_swapChainFormat;
        }
        VkExtent2D swapChainExtent() const {
            return m_swapChainExtent;
        }
        const std::vector<VkImage> &swapChainImages() const {
            return m_swapChainImages;
        }
        VkQueue graphicsQueue() const {
            return m_graphicsQueue;
        }
        VkQueue presentQueue() const {
            return m_presentQueue;
        }
        uint32_t graphicsQueueFamily() const {
            return m_graphicsQueueFamily;
        }
        uint32_t presentQueueFamily() const {
            return m_presentQueueFamily;
        }

        virtual void initRenderPipeline() = 0;
        virtual void render()             = 0;

        void paintEvent(QPaintEvent *event) override final;
        void resizeEvent(QResizeEvent *event) override;

    protected:
        virtual void initVulkan();
        virtual void createInstance();
        virtual void createSurface();
        virtual void setupDevice();
        virtual void createSwapChain();
        virtual void cleanup();

        bool checkValidationLayerSupport();
        std::vector<const char *> getRequiredExtensions();
        void findQueueFamilies();
        bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface);

    protected:
        VkInstance m_instance             = VK_NULL_HANDLE;
        VkSurfaceKHR m_surface            = VK_NULL_HANDLE;
        VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
        VkDevice m_device                 = VK_NULL_HANDLE;
        VkSwapchainKHR m_swapChain        = VK_NULL_HANDLE;
        std::vector<VkImage> m_swapChainImages;
        VkFormat m_swapChainFormat;
        VkExtent2D m_swapChainExtent;

        VkQueue m_graphicsQueue        = VK_NULL_HANDLE;
        VkQueue m_presentQueue         = VK_NULL_HANDLE;
        uint32_t m_graphicsQueueFamily = UINT32_MAX;
        uint32_t m_presentQueueFamily  = UINT32_MAX;
    };
} // namespace Retoccilus::Core::AbstractClasses::SceneWidget

#endif // ABSTRACT_SCENE_WIDGET_H