// =========== Quaternion.h ============ Re-Implementation of Assimp Quaternion ========= *- C++ -*
// // Original by Assimp Team, license under BSD 3-Clause License. Re-Implement by Rakyuu Studio.
// Copyright (c) 2023-2024 Rakyuu Studio, all rights reserved.
// SPDX-License-Identifier: LicenseRef-Apache-2.0-WITH-Retoccilus-Exception
// ================================================================================================
// //

#ifndef QUATERNION_INCLUDE_H
#define QUATERNION_INCLUDE_H

namespace Retoccilus::Engine3D::ModelImporter {
    template <typename Tp> class RtMIVector3D_Template;
    template <typename Tp> class RtMIMatrix3x3_Template;
    template <typename Tp> class RtMIMatrix4x4_Template;

    const double g_epsilon = 1e-6;

    template <typename Tp> class RtMIQuaternion_Template {
    public:
        RtMIQuaternion_Template() noexcept;
        RtMIQuaternion_Template(Tp pw, Tp px, Tp py, Tp pz);
        explicit RtMIQuaternion_Template(const RtMIMatrix3x3_Template<Tp> &pRotMatrix);
        explicit RtMIQuaternion_Template(RtMIVector3D_Template<Tp> normalized);

        RtMIQuaternion_Template(Tp roty, Tp rotz, Tp rotx);
        RtMIQuaternion_Template(RtMIVector3D_Template<Tp> axis, Tp angle);
        RtMIMatrix3x3_Template<Tp> getMatrix() const;

        bool operator==(const RtMIQuaternion_Template &o) const;
        bool operator!=(const RtMIQuaternion_Template &o) const;

        RtMIQuaternion_Template &operator*=(const RtMIMatrix4x4_Template<Tp> &mat);

        bool equal(const RtMIQuaternion_Template &o, Tp epsilon = g_epsilon) const;
        RtMIQuaternion_Template &normalize();

        RtMIQuaternion_Template &conjugate();
        RtMIVector3D_Template<Tp> rotate(const RtMIVector3D_Template<Tp> &in) const;
        RtMIQuaternion_Template operator*(const RtMIQuaternion_Template &two) const;

        static void interpolate(RtMIQuaternion_Template &pOut,
                                const RtMIQuaternion_Template &pStart,
                                const RtMIQuaternion_Template &pEnd, Tp pFactor);

        Tp w;
        Tp x;
        Tp y;
        Tp z;
    };
} // namespace Retoccilus::Engine3D::ModelImporter

#include "Quaternion.inl"

#endif // QUATERNION_INCLUDE_H