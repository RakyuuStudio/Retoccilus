#ifndef RT2DSCENEWIDGET_H
#define RT2DSCENEWIDGET_H

#include <memory>

namespace Retoccilus::Core {

    class Rt2DSceneWidget {
      public:
        Rt2DSceneWidget();
        ~Rt2DSceneWidget();

        void renderSprite(float x, float y, float scaleX, float scaleY, float rotation);

      private:
        struct Impl;
        std::unique_ptr<Impl> pImpl;
    };

} // namespace Retoccilus::Core

#endif // RT2DSCENEWIDGET_H
