#include "SceneWidget.h"

#include <QGuiApplication>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QSurfaceFormat>
#include <QVBoxLayout>
#include <QWindow>

namespace Retoccilus::Engine3D::Scene {
    RtSceneWidget::RtSceneWidget(QWidget *parent) : QWidget(parent) {
        m_vulkanInstance.setApiVersion(QVersionNumber(1, 3, 296));
        if (!m_vulkanInstance.create()) {
            qFatal("Failed to create Vulkan instance");
        }

        m_vulkanWindow = new QVulkanWindow();
        m_vulkanWindow->setVulkanInstance(&m_vulkanInstance);
        QWidget *container = QWidget::createWindowContainer(m_vulkanWindow, this);
        auto *layout = new QVBoxLayout(this);
        layout->addWidget(container);
        setLayout(layout);
    }

    RtSceneWidgetRenderer::RtSceneWidgetRenderer(QVulkanWindow *w) : m_window(w) {}

    void RtSceneWidgetRenderer::initResources() {
        // Initialize Resources
    }

    void RtSceneWidgetRenderer::startNextFrame() {
        // Render Frame
        m_window->frameReady();
        m_window->requestUpdate();
    }
} // namespace Retoccilus::Engine3D::Scene