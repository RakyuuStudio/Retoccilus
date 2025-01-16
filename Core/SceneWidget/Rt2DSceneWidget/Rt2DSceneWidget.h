#ifndef RT2DSCENEWIDGET_H
#define RT2DSCENEWIDGET_H

#include <memory>
#include <string>

namespace Retoccilus::Core {

    class Sprite {
      public:
        Sprite(const std::string &texturePath);
        ~Sprite();

        // 变换操作
        void move(float dx, float dy);
        void setPosition(float x, float y);
        void getPosition(float &x, float &y) const;

        void  rotate(float degrees);
        void  setRotation(float degrees);
        float getRotation() const;

        void scale(float sx, float sy);
        void setScale(float sx, float sy);
        void getScale(float &sx, float &sy) const;

        // 禁止拷贝
        Sprite(const Sprite &) = delete;
        Sprite &operator=(const Sprite &) = delete;

      private:
        friend class Rt2DSceneWidget;
        struct Impl;
        std::unique_ptr<Impl> pImpl;
    };

    class Rt2DSceneWidget {
      public:
        Rt2DSceneWidget(uint32_t width, uint32_t height, const std::string &title);
        ~Rt2DSceneWidget();

        // Sprite管理
        Sprite *createSprite(const std::string &texturePath);
        void    removeSprite(Sprite *sprite);

        // 场景控制
        void update(); // 更新场景
        void render(); // 渲染一帧

        // 窗口控制
        bool shouldClose() const;
        void setWindowSize(uint32_t width, uint32_t height);

        // 禁止拷贝
        Rt2DSceneWidget(const Rt2DSceneWidget &) = delete;
        Rt2DSceneWidget &operator=(const Rt2DSceneWidget &) = delete;

      private:
        struct Impl;
        std::unique_ptr<Impl> pImpl;
    };

} // namespace Retoccilus::Core

#endif // RT2DSCENEWIDGET_H
