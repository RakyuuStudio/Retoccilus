#ifndef MODEL_IMPORTER_UTILS_VERTEX_TYPE
#define MODEL_IMPORTER_UTILS_VERTEX_TYPE

#include <vulkan/vulkan.h>
#include <QVector3D>
#include <QVector2D>

namespace Retoccilus::Engine::Support3D::Utility {
    /**
     * @brief Struct RtVertex. Store the Vertex information of model.
     * Use the `GetSize()` method to get the vertex size, like this:
     *
     * @code
     * int main() {
     *     RtVertex vertex;
     *     vertex.Position = QVector3D(0.0f, 0.0f, 0.0f);
     *     vertex.TextureCoordinate = QVector2D(0.0f, 0.0f);
     *     vertex.Normal = QVector3D(0.0f, 0.0f, 1.0f);
     *
     *     int size = vertex.GetSize();
     * }
     * @endcode
     */
    struct RtVertex {
        QVector3D Position;
        QVector2D TextureCoordinate;
        QVector3D Normal;

        static unsigned int GetSize() {
            return sizeof(Position) + sizeof(TextureCoordinate) + sizeof(Normal);
        }
    };
}

#endif