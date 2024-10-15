#ifndef RTMI_MESH_H
#define RTMI_MESH_H

#include "ModelImporter/Utility/Container/ColorRGBA.h"
#include "ModelImporter/Utility/Container/Matrix4x4.h"
#include "ModelImporter/Utility/Tools/AxisAlignedBoundingBox.h"
#include "ModelImporter/Utility/Tools/Magic_Numbers.h"
#include "ModelImporter/Utility/Tools/RetoMIString.h"

#include <cstring>
#include <unordered_set>

namespace Retoccilus::Engine3D::ModelImporter {
    struct RtMIFace {
        unsigned int m_indicesNum;
        unsigned int *m_indices;

        RtMIFace() noexcept : m_indicesNum(0), m_indices(nullptr) {
        }
        RtMIFace(const RtMIFace &original) : m_indicesNum(0), m_indices(nullptr) {
            *this = original;
        }
        ~RtMIFace() {
            delete[] m_indices;
        }
        RtMIFace &operator=(const RtMIFace &original) {
            if (&original == this) {
                // Bro why you want to give yourself to yourself? It's definitely a paradox.
                return *this;
            }

            delete[] m_indices;
            m_indicesNum = original.m_indicesNum;
            if (m_indicesNum) {
                m_indices = new unsigned int[m_indicesNum];
                std::memcpy(m_indices, original.m_indices, m_indicesNum * sizeof(unsigned int));
            } else {
                m_indices = nullptr;
            }

            return *this;
        }

        bool operator==(const RtMIFace &original) const {
            if (m_indices == original.m_indices) {
                return true;
            }

            if (m_indices != nullptr && m_indicesNum != original.m_indicesNum) {
                return false;
            }

            if (m_indices == nullptr) {
                return false;
            }

            for (unsigned int i = 0; i < this->m_indicesNum; i++) {
                if (m_indices[i] != original.m_indices[i]) {
                    return false;
                }
            }

            return true;
        }

        bool operator!=(const RtMIFace &original) const {
            return !(*this == original);
        }
    };

    struct RtMIVertexWeight {
        unsigned int m_vertexId;
        float m_weight;

        RtMIVertexWeight() noexcept : m_vertexId(0), m_weight(0.0f) {
        }
        RtMIVertexWeight(unsigned int id, float w) : m_vertexId(id), m_weight(w) {
        }

        bool operator==(const RtMIVertexWeight &other) const {
            return m_vertexId == other.m_vertexId && m_weight == other.m_weight;
        }

        bool operator!=(const RtMIVertexWeight &other) const {
            return !(*this == other);
        }
    };

    struct RtMISceneNode;

    struct RtMIBone {
        RetoMIString m_name;
        unsigned int m_numWeights;
        RtMISceneNode *m_armature;
        RtMISceneNode *m_node;
        RtMIVertexWeight *m_weights;
        RtMIMatrix4x4_Template<float> m_offsetMatrix;

        RtMIBone() noexcept
            : m_name(),
              m_numWeights(0),
              m_armature(nullptr),
              m_node(nullptr),
              m_weights(nullptr),
              m_offsetMatrix() {
        }

        void copyVertexWeightsFromRhsToLhs(const RtMIBone &rhs) {
            if (rhs.m_weights == nullptr || rhs.m_numWeights == 0) {
                m_weights    = nullptr;
                m_numWeights = 0;
                return;
            }

            m_numWeights = rhs.m_numWeights;
            if (m_weights) {
                delete[] m_weights;
            }

            m_weights = new RtMIVertexWeight[m_numWeights];
            std::memcpy(m_weights, rhs.m_weights, m_numWeights * sizeof(RtMIVertexWeight));
        }

        RtMIBone(const RtMIBone &original)
            : m_name(original.m_name),
              m_numWeights(original.m_numWeights),
              m_armature(nullptr),
              m_node(nullptr),
              m_weights(nullptr),
              m_offsetMatrix(original.m_offsetMatrix) {
            copyVertexWeightsFromRhsToLhs(original);
        }

        RtMIBone &operator=(const RtMIBone &original) {
            if (&original == this) {
                return *this;
            }

            m_name         = original.m_name;
            m_numWeights   = original.m_numWeights;
            m_offsetMatrix = original.m_offsetMatrix;
            copyVertexWeightsFromRhsToLhs(original);

            return *this;
        }

        bool operator==(const RtMIBone &other) const {
            if (m_name != other.m_name || m_numWeights != other.m_numWeights) {
                return false;
            }

            for (auto i = 0; i < m_numWeights; i++) {
                if (m_weights[i] != other.m_weights[i]) {
                    return false;
                }
            }

            return true;
        }

        ~RtMIBone() {
            delete[] m_weights;
        }
    };

    enum Rt_GeometryPrimitiveTypes {
        // The One-Dimensional Point
        Rt_PrimitiveGeometry_Point = 1,

        // The Two-Dimensional Line (with 2 points and connected by a line)
        Rt_PrimitiveGeometry_Line = 2,

        // The Two-Dimensional Triangle (with 3 points and connected each other with 3 lines)
        Rt_PrimitiveGeometry_Triangle = 3,

        // The Two-Dimensional Polygon (with any points (greater than 4) and connected each other
        // with lines)
        Rt_PrimitiveGeometry_Polygon = 8,

        // The Two-Dimensional N-Gon "means a polygon with n-lines" (with n points and connected
        // each other with n
        // lines)
        Rt_PrimitiveGeometry_ngon = 16,
    };

    struct Rt_AnimationMesh {
        RetoMIString m_name;
        RtMIVector3D_Template<float> *m_vertices;
        RtMIVector3D_Template<float> *m_normals;
        RtMIVector3D_Template<float> *m_tangents;
        RtMIVector3D_Template<float> *m_bi_tangents;

        RtMIColorRGBA_Template<float> *m_colors[MAX_COLOR_SET];
        RtMIVector3D_Template<float> *m_textureCoordinates[MAX_TEXTURE_COORDINATES];

        unsigned int m_numVertices;
        float m_weight;

        Rt_AnimationMesh() noexcept
            : m_vertices(nullptr),
              m_normals(nullptr),
              m_tangents(nullptr),
              m_bi_tangents(nullptr),
              m_colors{nullptr},
              m_textureCoordinates{nullptr},
              m_numVertices(0u),
              m_weight(0.0f) {
        }

        ~Rt_AnimationMesh() {
            delete[] m_vertices;
            delete[] m_normals;
            delete[] m_tangents;
            delete[] m_bi_tangents;

            // TODO: Might cause wrong result
            for (auto &m_color : m_colors) {
                delete[] m_color;
            }

            for (auto &m_textureCoordinate : m_textureCoordinates) {
                delete[] m_textureCoordinate;
            }
        }

        [[nodiscard]] bool hasPosition() const {
            return (m_vertices != nullptr);
        }

        [[nodiscard]] bool hasNormals() const {
            return (m_normals != nullptr);
        }

        [[nodiscard]] bool hasTangentAndBiTangents() const {
            return (m_tangents != nullptr);
        }

        [[nodiscard]] bool hasVertexColors(unsigned int pIndex) const {
            if (pIndex >= MAX_COLOR_SET) {
                return false;
            } else {
                return m_colors[pIndex] != nullptr;
            }
        }

        [[nodiscard]] bool hasTextureCoordinates(unsigned int pIndex) const {
            if (pIndex >= MAX_TEXTURE_COORDINATES) {
                return false;
            } else {
                return m_textureCoordinates[pIndex] != nullptr;
            }
        }
    };

    enum Rt_MorphingType {
        Rt_MorphingMethod_Unknown                    = 0,
        Rt_MorphingMethod_Vertex_Blend_Interpolation = 1,
        Rt_MorphingMethod_Morph_Normalized           = 2,
        Rt_MorphingMethod_Morph_Relative             = 3
    };

    struct RtMIMesh {
        unsigned int m_primitiveTypes;
        unsigned int m_numVertices;
        unsigned int m_numFaces;
        unsigned int m_numBones;
        unsigned int m_numUVComponent[MAX_TEXTURE_COORDINATES];
        unsigned int m_materialIndex;
        unsigned int m_numAnimationMeshes;

        RtMIVector3D_Template<float> *m_vertices;
        RtMIVector3D_Template<float> *m_normals;
        RtMIVector3D_Template<float> *m_tangents;
        RtMIVector3D_Template<float> *m_bi_tangents;
        RtMIVector3D_Template<float> *m_textureCoordinates[MAX_TEXTURE_COORDINATES];
        RtMIColorRGBA_Template<float> *m_colors[MAX_COLOR_SET];
        RtMIFace *m_faces;
        RtMIBone **m_bones;
        RetoMIString m_name;
        Rt_AnimationMesh **m_animationMeshes;
        enum Rt_MorphingType m_morphingMethod;
        RtMIAxisAlignedBoundingBox m_aabb;
        RetoMIString **m_textureCoordinateNames;

        RtMIMesh() noexcept
            : m_primitiveTypes(0),
              m_numVertices(0),
              m_numFaces(0),
              m_vertices(nullptr),
              m_normals(nullptr),
              m_tangents(nullptr),
              m_bi_tangents(nullptr),
              m_colors{nullptr},
              m_textureCoordinates{nullptr},
              m_faces(nullptr),
              m_numUVComponent{0},
              m_numBones(0),
              m_materialIndex(0),
              m_numAnimationMeshes(0),
              m_aabb(),
              m_morphingMethod(Rt_MorphingMethod_Unknown),
              m_textureCoordinateNames(nullptr),
              m_bones(nullptr),
              m_animationMeshes(nullptr) {
        }

        ~RtMIMesh() {
            delete[] m_vertices;
            delete[] m_normals;
            delete[] m_tangents;
            delete[] m_bi_tangents;

            for (auto &m_textureCoordinate : m_textureCoordinates) {
                delete[] m_textureCoordinate;
            }

            if (m_textureCoordinateNames) {
                for (auto a = 0; a < MAX_TEXTURE_COORDINATES; a++) {
                    delete m_textureCoordinateNames[a];
                }
                delete[] m_textureCoordinateNames;
            }

            for (unsigned int a = 0; a < MAX_COLOR_SET; a++) {
                delete[] m_colors[a];
            }

            if (m_numBones && m_bones) {
                std::unordered_set<const RtMIBone *> bones;
                for (unsigned int a = 0; a < m_numBones; a++) {
                    if (m_bones[a]) {
                        bones.insert(m_bones[a]);
                    }
                }
                for (const RtMIBone *bone : bones) {
                    delete bone;
                }
                delete[] m_bones;
            }

            if (m_numAnimationMeshes && m_animationMeshes) {
                for (unsigned int a = 0; a < m_numAnimationMeshes; a++) {
                    delete m_animationMeshes[a];
                }
                delete[] m_animationMeshes;
            }

            delete[] m_faces;
        }

        [[nodiscard]] bool hasPositions() const {
            return m_vertices != nullptr && m_numVertices > 0;
        }

        [[nodiscard]] bool hasFaces() const {
            return m_faces != nullptr && m_numFaces > 0;
        }

        [[nodiscard]] bool hasNormals() const {
            return m_normals != nullptr && m_numVertices > 0;
        }

        [[nodiscard]] bool hasTangentsAndBiTangents() const {
            return m_tangents != nullptr && m_bi_tangents != nullptr && m_numVertices > 0;
        }

        [[nodiscard]] bool hasVertexColors(unsigned int index) const {
            if (index >= MAX_COLOR_SET) {
                return false;
            }
            return m_colors[index] != nullptr && m_numVertices > 0;
        }

        [[nodiscard]] bool hasTextureCoords(unsigned int index) const {
            if (index >= MAX_TEXTURE_COORDINATES) {
                return false;
            }
            return (m_textureCoordinates[index] != nullptr && m_numVertices > 0);
        }

        [[nodiscard]] unsigned int getNumUVChannels() const {
            unsigned int n(0);
            for (auto m_textureCoordinate : m_textureCoordinates) {
                if (m_textureCoordinate) {
                    ++n;
                }
            }

            return n;
        }

        [[nodiscard]] unsigned int getNumColorChannels() const {
            unsigned int n(0);
            while (n < MAX_COLOR_SET && m_colors[n]) {
                ++n;
            }
            return n;
        }

        [[nodiscard]] bool hasBones() const {
            return m_bones != nullptr && m_numBones > 0;
        }

        [[nodiscard]] bool HasTextureCoordsName(unsigned int pIndex) const {
            if (m_textureCoordinateNames == nullptr || pIndex >= MAX_TEXTURE_COORDINATES) {
                return false;
            }
            return m_textureCoordinateNames[pIndex] != nullptr;
        }

        void setTextureCoordsName(unsigned int pIndex, RetoMIString texCoordsName) {
            if (pIndex >= MAX_TEXTURE_COORDINATES) {
                return;
            }

            if (m_textureCoordinateNames == nullptr) {
                m_textureCoordinateNames = new RetoMIString *[MAX_TEXTURE_COORDINATES];
                for (size_t i = 0; i < MAX_TEXTURE_COORDINATES; ++i) {
                    m_textureCoordinateNames[i] = nullptr;
                }
            }

            if (texCoordsName.length() == 0) {
                delete m_textureCoordinateNames[pIndex];
                m_textureCoordinateNames[pIndex] = nullptr;
                return;
            }

            if (m_textureCoordinateNames[pIndex] == nullptr) {
                m_textureCoordinateNames[pIndex] = new RetoMIString(texCoordsName);
                return;
            }

            *m_textureCoordinateNames[pIndex] = texCoordsName;
        }

        [[nodiscard]] const RetoMIString *getTextureCoordsName(unsigned int index) const {
            if (m_textureCoordinateNames == nullptr || index >= MAX_TEXTURE_COORDINATES) {
                return nullptr;
            }

            return m_textureCoordinateNames[index];
        }
    };

    struct RtSkeletonBone {
        int m_parent;
        RtMISceneNode *m_armature;
        RtMISceneNode *m_node;

        unsigned int m_numWeights;
        RtMIMesh *m_meshID;
        RtMIVertexWeight *m_weights;

        RtMIMatrix4x4_Template<float> m_offsetMatrix;
        RtMIMatrix4x4_Template<float> m_localMatrix;

        RtSkeletonBone()
            : m_parent(-1),
              m_armature(nullptr),
              m_node(nullptr),
              m_numWeights(0),
              m_meshID(nullptr),
              m_weights(nullptr),
              m_offsetMatrix(),
              m_localMatrix() {
        }

        // TODO: May cause errors.
        explicit RtSkeletonBone(unsigned int parent)
            : m_parent(static_cast<int>(parent)),
              m_armature(nullptr),
              m_node(nullptr),
              m_numWeights(0),
              m_meshID(nullptr),
              m_weights(nullptr),
              m_offsetMatrix(),
              m_localMatrix() {
        }

        ~RtSkeletonBone() {
            delete[] m_weights;
            m_weights = nullptr;
        }
    };

    struct RtMISkeleton {
        RetoMIString m_name;
        unsigned int m_numBones;
        RtSkeletonBone **m_bones;

        RtMISkeleton() noexcept : m_name(), m_numBones(0), m_bones(nullptr) {
        }

        ~RtMISkeleton() {
            delete[] m_bones;
        }
    };
} // namespace Retoccilus::Engine3D::ModelImporter

#endif // RTMI_MESH_H