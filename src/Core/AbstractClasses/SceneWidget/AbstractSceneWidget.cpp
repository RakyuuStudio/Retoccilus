#include <algorithm>
#include <qlogging.h>
#include <vector>
#include <vulkan/vulkan_core.h>
#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <windows.h>
#endif

#include <vulkan/vulkan.h>
#ifdef _WIN32
#include <vulkan/vulkan_win32.h>
#endif

#include "Core/AbstractClasses/SceneWidget/AbstractSceneWidget.hpp"
#include <GLFW/glfw3.h>
#include <QPaintEvent>
#include <QVulkanInstance>
#include <stdexcept>

namespace Retoccilus::Core::AbstractClasses::SceneWidget {
    RAbstractSceneWidget::RAbstractSceneWidget(QWidget *parent) : QWidget(parent) {
        setAttribute(Qt::WA_NativeWindow);
        setAttribute(Qt::WA_PaintOnScreen);
    }

    RAbstractSceneWidget::~RAbstractSceneWidget() {
        cleanup();
    }

    void RAbstractSceneWidget::initVulkan() {
        qDebug() << "Initializing Vulkan...";
        if (m_instance != VK_NULL_HANDLE) {
            qDebug() << "Warning: Vulkan instance already initialized";
            cleanup();
        }

        createInstance();
        if (m_instance == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to create Vulkan instance");
        }

        createSurface();
        if (m_surface == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to create surface");
        }

        setupDevice();
        if (m_device == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to setup device");
        }

        createSwapChain();
        if (m_swapChain == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to create swap chain");
        }

        qDebug() << "Vulkan initialization completed successfully";
    }

    void RAbstractSceneWidget::createInstance() {
        VkApplicationInfo appInfo{};
        appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName   = "Vulkan Widget";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 3, 0);
        appInfo.pEngineName        = "No Engine";
        appInfo.engineVersion      = VK_MAKE_VERSION(1, 3, 0);
        appInfo.apiVersion         = VK_API_VERSION_1_3;

        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#ifdef _DEBUG
#endif

#ifdef _WIN32
        extensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
        extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
#endif

        VkInstanceCreateInfo createInfo{};
        createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo        = &appInfo;
        createInfo.enabledExtensionCount   = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

#ifdef _DEBUG
        std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
        createInfo.enabledLayerCount               = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames             = validationLayers.data();
#else
        createInfo.enabledLayerCount = 0;
#endif

        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance");
        }
    }

    void RAbstractSceneWidget::createSurface() {
#ifdef _WIN32
        VkWin32SurfaceCreateInfoKHR createInfo{};
        createInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hwnd      = (HWND)winId();
        createInfo.hinstance = GetModuleHandle(nullptr);

        if (vkCreateWin32SurfaceKHR(m_instance, &createInfo, nullptr, &m_surface) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create window surface");
        }
#elif defined(__linux__)
        // TODO: This should be implemented in the future.
#endif
    }

    void RAbstractSceneWidget::setupDevice() {
        qDebug() << "Setting up device...";
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            throw std::runtime_error("Failed to find GPUs with Vulkan support");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(m_instance, &deviceCount, devices.data());

        m_physicalDevice = devices[0];
        if (m_physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to select physical device");
        }
        qDebug() << "Selected physical device:" << m_physicalDevice;
        
        if (m_surface == VK_NULL_HANDLE) {
            throw std::runtime_error("Surface must be created before setting up device");
        }

        findQueueFamilies();
        qDebug() << "Queue families found - Graphics:" << m_graphicsQueueFamily 
                 << "Present:" << m_presentQueueFamily;

        if (!isDeviceSuitable(m_physicalDevice, m_surface)) {
            throw std::runtime_error("Selected physical device is not suitable");
        }

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {m_graphicsQueueFamily, m_presentQueueFamily};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;

        std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        #ifdef _DEBUG
            std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        #else
            createInfo.enabledLayerCount = 0;
        #endif

        qDebug() << "Creating logical device with" << queueCreateInfos.size() << "queue families";
        VkResult result = vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device);
        if (result != VK_SUCCESS) {
            qDebug() << "Failed to create logical device with error code:" << result;
            throw std::runtime_error("Failed to create logical device");
        }

        if (m_device == VK_NULL_HANDLE) {
            throw std::runtime_error("Device creation succeeded but handle is null");
        }

        vkGetDeviceQueue(m_device, m_graphicsQueueFamily, 0, &m_graphicsQueue);
        vkGetDeviceQueue(m_device, m_presentQueueFamily, 0, &m_presentQueue);

        if (m_graphicsQueue == VK_NULL_HANDLE || m_presentQueue == VK_NULL_HANDLE) {
            throw std::runtime_error("Failed to get device queues");
        }

        qDebug() << "Device created successfully. Graphics queue:" << m_graphicsQueue 
                 << "Present queue:" << m_presentQueue;
    }

    void RAbstractSceneWidget::findQueueFamilies() {
        m_graphicsQueueFamily = UINT32_MAX;
        m_presentQueueFamily = UINT32_MAX;

        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &queueFamilyCount,
                                                queueFamilies.data());

        for (uint32_t i = 0; i < queueFamilyCount; i++) {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, m_surface, &presentSupport);

            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_graphicsQueueFamily = i;
            }

            if (presentSupport) {
                m_presentQueueFamily = i;
            }

            if (m_graphicsQueueFamily != UINT32_MAX && m_presentQueueFamily != UINT32_MAX) {
                break;
            }
        }

        if (m_graphicsQueueFamily == UINT32_MAX || m_presentQueueFamily == UINT32_MAX) {
            throw std::runtime_error("Failed to find suitable queue families");
        }

        qDebug() << "Found queue families - Graphics:" << m_graphicsQueueFamily 
                 << "Present:" << m_presentQueueFamily;
    }

    void RAbstractSceneWidget::createSwapChain() {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        bool swapChainSupported = false;
        for (const auto& extension : availableExtensions) {
            if (strcmp(extension.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
                swapChainSupported = true;
                break;
            }
        }

        if (!swapChainSupported) {
            throw std::runtime_error("Device does not support swap chain extension");
        }

        VkSurfaceCapabilitiesKHR capabilities;
        if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_physicalDevice, m_surface, &capabilities) != VK_SUCCESS) {
            throw std::runtime_error("Failed to get surface capabilities");
        }

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, nullptr);
        if (formatCount == 0) {
            throw std::runtime_error("No surface formats available");
        }

        std::vector<VkSurfaceFormatKHR> formats(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(m_physicalDevice, m_surface, &formatCount, formats.data());

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, nullptr);
        if (presentModeCount == 0) {
            throw std::runtime_error("No presentation modes available");
        }

        std::vector<VkPresentModeKHR> presentModes(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(m_physicalDevice, m_surface, &presentModeCount, presentModes.data());

        if (m_instance == VK_NULL_HANDLE || m_device == VK_NULL_HANDLE) {
            throw std::runtime_error("Vulkan instance or device is not initialized");
        }

        if (m_surface == VK_NULL_HANDLE) {
            throw std::runtime_error("Vulkan surface is not created");
        }

        VkSurfaceFormatKHR surfaceFormat = formats[0];
        for (const auto &format : formats) {
            if (format.format == VK_FORMAT_B8G8R8_SRGB &&
                format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                surfaceFormat = format;
                break;
            }
        }

        VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
        for (const auto &mode : presentModes) {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR) {
                presentMode = mode;
                break;
            }
        }

        VkExtent2D extent;
        if (capabilities.currentExtent.width != UINT32_MAX) {
            extent = capabilities.currentExtent;
        } else {
            extent.width =
                std::clamp(static_cast<uint32_t>(width()), capabilities.minImageExtent.width,
                           capabilities.maxImageExtent.width);
            extent.height =
                std::clamp(static_cast<uint32_t>(height()), capabilities.minImageExtent.height,
                           capabilities.maxImageExtent.height);
        }

        uint32_t imageCount = capabilities.minImageCount + 1;
        if (capabilities.maxImageCount > 0) {
            imageCount = std::min(imageCount, capabilities.maxImageCount);
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface          = m_surface;
        createInfo.minImageCount    = imageCount;
        createInfo.imageFormat      = surfaceFormat.format;
        createInfo.imageColorSpace  = surfaceFormat.colorSpace;
        createInfo.imageExtent      = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = {m_graphicsQueueFamily, m_presentQueueFamily};

        if (m_graphicsQueueFamily != m_presentQueueFamily) {
            createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices   = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices   = nullptr;
        }

        createInfo.preTransform   = capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode    = presentMode;
        createInfo.clipped        = VK_TRUE;
        createInfo.oldSwapchain   = VK_NULL_HANDLE;

        qDebug() << "Creating swap chain with parameters below...: ";
        qDebug() << "Parameter [0]: minImageCount: " << imageCount;
        qDebug() << "Parameter [1]: imageFormat: " << surfaceFormat.format;
        qDebug() << "Parameter [2]: imageExtent: [Width] - " << extent.width << "[Height] - "
                 << extent.height;
        qDebug() << "Parameter [3]: presentMode: " << presentMode;
        qDebug() << "Parameter [4]: graphicsQueueFamily: " << m_graphicsQueueFamily;
        qDebug() << "Parameter [5]: presentQueueFamily: " << m_presentQueueFamily;

        if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create swap chain.");
        }

        vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, nullptr);
        m_swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_device, m_swapChain, &imageCount, m_swapChainImages.data());

        m_swapChainFormat = surfaceFormat.format;
        m_swapChainExtent = extent;
    }

    void RAbstractSceneWidget::cleanup() {
        if (m_device != VK_NULL_HANDLE) {
            qDebug() << "1";
            vkDeviceWaitIdle(m_device);
            qDebug() << "Device wait idle completed";
        }

        if (m_device != VK_NULL_HANDLE && m_swapChain != VK_NULL_HANDLE) {
            vkDestroySwapchainKHR(m_device, m_swapChain, nullptr);
            m_swapChain = VK_NULL_HANDLE;
            qDebug() << "Swap chain destroyed";
        }

        if (m_device != VK_NULL_HANDLE) {
            vkDestroyDevice(m_device, nullptr);
            m_device = VK_NULL_HANDLE;
            qDebug() << "Device destroyed";
        }

        if (m_surface != VK_NULL_HANDLE) {
            vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
            m_surface = VK_NULL_HANDLE;
        }

        if (m_instance != VK_NULL_HANDLE) {
            vkDestroyInstance(m_instance, nullptr);
            m_instance = VK_NULL_HANDLE;
        }
    }

    void RAbstractSceneWidget::paintEvent(QPaintEvent *event) {
        render();
    }

    void RAbstractSceneWidget::resizeEvent(QResizeEvent *event) {
        if (m_device != VK_NULL_HANDLE) {
            QSize newSize = event->size();
            if (newSize.width() > 0 && newSize.height() > 0 &&
                (newSize.width() != m_swapChainExtent.width || 
                 newSize.height() != m_swapChainExtent.height)) {
                qDebug() << "Recreating swap chain due to resize:" << newSize;
                vkDeviceWaitIdle(m_device);
                createSwapChain();
                initRenderPipeline();
            }
        }
        QWidget::resizeEvent(event);
    }

    bool RAbstractSceneWidget::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        bool hasGraphicsQueue = false;
        bool hasPresentQueue = false;
        
        for (uint32_t i = 0; i < queueFamilyCount; i++) {
            if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                hasGraphicsQueue = true;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (presentSupport) {
                hasPresentQueue = true;
            }
        }

        return hasGraphicsQueue && hasPresentQueue;
    }
} // namespace Retoccilus::Core::AbstractClasses::SceneWidget