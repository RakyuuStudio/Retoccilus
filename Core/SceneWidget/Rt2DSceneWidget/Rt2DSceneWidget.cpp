#include "Rt2DSceneWidget.h"
#include "Core/VulkanWrapper/VulkanWrapper.h"
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Retoccilus::Core {
    // Sprite Implementation
    struct Sprite::Impl {
        glm::vec2   position{0.0f, 0.0f};
        glm::vec2   scale{1.0f, 1.0f};
        float       rotation{0.0f}; // 角度制
        std::string texturePath;
        uint32_t    textureId = 0; // 添加纹理ID

        Impl(const std::string &path) : texturePath(path) {}

        glm::mat4 getTransformMatrix() const {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(position, 0.0f));
            model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(scale, 1.0f));
            return model;
        }
    };

    Sprite::Sprite(const std::string &texturePath)
        : pImpl(std::make_unique<Impl>(texturePath)) {}

    Sprite::~Sprite() = default;

    void Sprite::move(float dx, float dy) {
        pImpl->position.x += dx;
        pImpl->position.y += dy;
    }

    void Sprite::setPosition(float x, float y) {
        pImpl->position = glm::vec2(x, y);
    }

    void Sprite::getPosition(float &x, float &y) const {
        x = pImpl->position.x;
        y = pImpl->position.y;
    }

    void Sprite::rotate(float degrees) {
        pImpl->rotation += degrees;
        while (pImpl->rotation >= 360.0f)
            pImpl->rotation -= 360.0f;
        while (pImpl->rotation < 0.0f)
            pImpl->rotation += 360.0f;
    }

    void Sprite::setRotation(float degrees) {
        pImpl->rotation = degrees;
        while (pImpl->rotation >= 360.0f)
            pImpl->rotation -= 360.0f;
        while (pImpl->rotation < 0.0f)
            pImpl->rotation += 360.0f;
    }

    float Sprite::getRotation() const {
        return pImpl->rotation;
    }

    void Sprite::scale(float sx, float sy) {
        pImpl->scale.x *= sx;
        pImpl->scale.y *= sy;
    }

    void Sprite::setScale(float sx, float sy) {
        pImpl->scale = glm::vec2(sx, sy);
    }

    void Sprite::getScale(float &sx, float &sy) const {
        sx = pImpl->scale.x;
        sy = pImpl->scale.y;
    }

    // Rt2DSceneWidget Implementation
    struct Rt2DSceneWidget::Impl {
        std::unique_ptr<VulkanWrapper>       renderer;
        std::vector<std::unique_ptr<Sprite>> sprites;
        uint32_t                             width;
        uint32_t                             height;

        Impl(uint32_t w, uint32_t h, const std::string &title)
            : width(w), height(h) {
            renderer = std::make_unique<VulkanWrapper>(w, h, title);
            renderer->initialize();
        }

        void loadSpriteTexture(Sprite *sprite) {
            if (sprite->pImpl->textureId == 0) {
                renderer->loadTexture(sprite->pImpl->texturePath,
                                      sprite->pImpl->textureId);
            }
        }

        void updateSprites() {
            for (const auto &sprite : sprites) {
                // 确保纹理已加载
                loadSpriteTexture(sprite.get());

                // 获取并设置变换矩阵
                glm::mat4 transform = sprite->pImpl->getTransformMatrix();
                renderer->setTransformMatrix(transform);

                // TODO: 在这里添加实际的绘制调用
            }
        }

        void renderFrame() {
            renderer->render();
        }

        ~Impl() {
            // 清理所有已加载的纹理
            for (const auto &sprite : sprites) {
                if (sprite->pImpl->textureId != 0) {
                    renderer->unloadTexture(sprite->pImpl->textureId);
                }
            }
        }
    };

    Rt2DSceneWidget::Rt2DSceneWidget(uint32_t width, uint32_t height,
                                     const std::string &title)
        : pImpl(std::make_unique<Impl>(width, height, title)) {
    }

    Rt2DSceneWidget::~Rt2DSceneWidget() = default;

    Sprite *Rt2DSceneWidget::createSprite(const std::string &texturePath) {
        auto    sprite = std::make_unique<Sprite>(texturePath);
        Sprite *ptr = sprite.get();
        pImpl->sprites.push_back(std::move(sprite));
        return ptr;
    }

    void Rt2DSceneWidget::removeSprite(Sprite *sprite) {
        auto it = std::find_if(pImpl->sprites.begin(), pImpl->sprites.end(),
                               [sprite](const std::unique_ptr<Sprite> &s) {
                                   return s.get() == sprite;
                               });

        if (it != pImpl->sprites.end()) {
            pImpl->sprites.erase(it);
        }
    }

    void Rt2DSceneWidget::update() {
        pImpl->updateSprites();
    }

    void Rt2DSceneWidget::render() {
        pImpl->renderFrame();
    }

    bool Rt2DSceneWidget::shouldClose() const {
        return pImpl->renderer->windowShouldClose();
    }

    void Rt2DSceneWidget::setWindowSize(uint32_t width, uint32_t height) {
        pImpl->width = width;
        pImpl->height = height;
        pImpl->renderer->resizeWindow(width, height);
    }

} // namespace Retoccilus::Core
