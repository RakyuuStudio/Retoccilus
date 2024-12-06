#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <windows.h>
#endif

#include <vulkan/vulkan.h>
#ifdef _WIN32
#include <vulkan/vulkan_win32.h>
#endif

#include <QApplication>
#include <Editor/SceneWidget/SceneWidget2D.hpp>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <QMessageBox>
#include <QVulkanInstance>
#include <QDir>
#include <QDebug>

using namespace Retoccilus::Editor::SceneWidget;

int main(int argc, char *argv[]) {
    // 首先创建QApplication
    QApplication a(argc, argv);

    try {
        // 初始化GLFW
        if (!glfwInit()) {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        // 设置GLFW不创建OpenGL上下文
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        // 创建Vulkan实例
        QVulkanInstance instance;
        
        // 设置Vulkan实例的属性
        instance.setLayers(QByteArrayList() << "VK_LAYER_KHRONOS_validation");
        
        // 设置扩展
        QByteArrayList extensions = {
            VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
            VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#endif
            VK_EXT_DEBUG_UTILS_EXTENSION_NAME
        };
        instance.setExtensions(extensions);

        // 创建Vulkan实例
        if (!instance.create()) {
            throw std::runtime_error("Failed to create Vulkan instance");
        }

        // 打印当前工作目录和着色器文件位置
        qDebug() << "Current working directory:" << QDir::currentPath();
        qDebug() << "Looking for shaders in:" << QDir::currentPath() + "/shaders/";
        
        // 检查着色器文件是否存在
        QFile vertFile(QString(QDir::currentPath() + "/shaders/vert.spv"));
        QFile fragFile(QString(QDir::currentPath() + "/shaders/frag.spv"));
        
        qDebug() << "Vertex shader exists:" << vertFile.exists();
        qDebug() << "Fragment shader exists:" << fragFile.exists();

        // 创建并显示窗口
        RSceneWidget2D w;
        w.resize(800, 600);
        w.show();

        int result = a.exec();

        // 清理资源
        glfwTerminate();
        return result;

    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Error", e.what());
        glfwTerminate();
        return -1;
    }
} 