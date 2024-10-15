#ifndef RETO_MATH_AXIS_ALIGNED_BOUNDING_BOX_H
#define RETO_MATH_AXIS_ALIGNED_BOUNDING_BOX_H

#include "ModelImporter/Utility/Container/Vector3D.h"

namespace Retoccilus::Engine3D::ModelImporter {
    struct RtMIAxisAlignedBoundingBox {
        RtMIVector3D_Template<float> m_min;
        RtMIVector3D_Template<float> m_max;

        RtMIAxisAlignedBoundingBox() = default;
        RtMIAxisAlignedBoundingBox(const RtMIVector3D_Template<float> &min,
                                   const RtMIVector3D_Template<float> &max)
            : m_min(min),
              m_max(max) {
        }

        ~RtMIAxisAlignedBoundingBox() = default;
    };
} // namespace Retoccilus::Engine3D::ModelImporter

#endif // RETO_MATH_AXIS_ALIGNED_BOUNDING_BOX_H