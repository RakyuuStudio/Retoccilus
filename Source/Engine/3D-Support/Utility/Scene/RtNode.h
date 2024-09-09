#ifndef RTNODE_H
#define RTNODE_H

#include <QString>
#include "Math/Quaternion.h"
#include "Math/Matrix4x4.h"

namespace Retoccilus::Engine::ThreeDimensionalSupport::Utility {
    struct RtNode {
        QString m_Name;                             // Node Name
        RtMatrix4x4T<rt_real> matrixTransformation; // Transformation Data of parent node
        RtNode *m_Parent;                           // Parent Node Instance
        unsigned int m_ChildCount;                  // Children Node Count of this Node
        RtNode** m_Children;                        // Children Node Instance of This Node
        unsigned int m_MeshCount;                   // Mesh Count of this Node
        unsigned int *m_Meshes;                     // Mesh Instance of this Node

    };
}

#endif // RTNODE_H