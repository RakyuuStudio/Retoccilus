#include "Rt2DSceneWidget.h"
#include "Core/VulkanWrapper/VulkanWrapper.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Retoccilus::Core {

    struct Rt2DSceneWidget::Impl {
        std::shared_ptr<VulkanWrapper> renderer;

        struct SpriteTransform {
            glm::vec2 position;
            glm::vec2 scale;
            float     rotation;
        };

        void applyTransform(const SpriteTransform &transform) {
            // Create transformation matrix
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(transform.position, 0.0f));
            model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(transform.scale, 1.0f));

            // TODO: Send transformation matrix to Vulkan renderer
        }
    };

    Rt2DSceneWidget::Rt2DSceneWidget() : pImpl(std::make_unique<Impl>()) {
        // Initialize renderer
        pImpl->renderer = std::make_shared<VulkanWrapper>(800, 600, "2D Scene");
    }

    Rt2DSceneWidget::~Rt2DSceneWidget() = default;

    void Rt2DSceneWidget::renderSprite(float x, float y, float scaleX, float scaleY, float rotation) {
        Rt2DSceneWidget::Impl::SpriteTransform transform{
            .position = {x, y},
            .scale = {scaleX, scaleY},
            .rotation = rotation};

        pImpl->applyTransform(transform);
    }

} // namespace Retoccilus::Core
