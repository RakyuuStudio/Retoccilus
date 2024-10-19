#ifndef SCENE_WIDGET_INCLUDE
#define SCENE_WIDGET_INCLUDE

#include <QVulkanFunctions>
#include <QWidget>
#include <QVulkanWindowRenderer>
#include <QVulkanWindow>
#include <vulkan/vulkan.h>

namespace Retoccilus::Engine3D::Scene {
    class RtSceneWidgetRenderer : public QVulkanWindowRenderer {
    public:
        explicit RtSceneWidgetRenderer(QVulkanWindow *w);
        void initResources() override;
        void startNextFrame() override;
    private:
        QVulkanWindow *m_window;
    };

    class RtSceneWidget : public QWidget {
//        Q_OBJECT
    public:
        explicit RtSceneWidget(QWidget *parent = nullptr);
    protected:
        QVulkanWindowRenderer *createRenderer()  {
            return new RtSceneWidgetRenderer(m_vulkanWindow);
        }
    private:
        QVulkanInstance m_vulkanInstance;
        QVulkanWindow *m_vulkanWindow;
        RtSceneWidgetRenderer *m_renderer{};
    };
} // namespace Retoccilus::Engine3D::Scene

#endif // SCENE_WIDGET_INCLUDE