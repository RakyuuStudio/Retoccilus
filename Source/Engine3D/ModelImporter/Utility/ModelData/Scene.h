#ifndef RETO_MI_SCENE_INCLUDE_H
#define RETO_MI_SCENE_INCLUDE_H

#include <cstdlib>

#include "Metadata.h"
#include "ModelImporter/Utility/Container/Matrix3x3.h"
#include "ModelImporter/Utility/Container/Matrix4x4.h"
#include "ModelImporter/Utility/Tools/RetoMIString.h"

namespace Retoccilus::Engine3D::ModelImporter {
    struct RtMISceneNode {
        RetoMIString m_name;
        RtMIMatrix3x3_Template<float> m_transformation;
        RtMISceneNode *parent;
        unsigned int m_childrenCount;
        RtMISceneNode **m_children;
        unsigned int m_numMeshes;
        unsigned int *mMeshes;
        RtMIMetadata *m_metadata;

        RtMISceneNode();
        explicit RtMISceneNode(const std::string &name);
        ~RtMISceneNode();

        inline const RtMISceneNode *findNode(const RetoMIString &name) const { return findNode(name.getData()); }

        inline RtMISceneNode *findNode(const RetoMIString &name) { return findNode(name.getData()); }

        const RtMISceneNode *findNode(const char *name) const;
        RtMISceneNode *findNode(const char *name);

        void addChildren(unsigned int numChildren, RtMISceneNode **children);
    };

    struct RtMIScene {
        unsigned int m_flags;
        RtMISceneNode* m_rootNode;
        unsigned int m_numMeshes;
        RtMIMesh **m_meshes;
    };
} // namespace Retoccilus::Engine3D::ModelImporter

#endif // RETO_MI_SCENE_INCLUDE_H