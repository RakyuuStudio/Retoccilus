#ifndef MATRIX_4X4_INCLUDE_H
#define MATRIX_4X4_INCLUDE_H

#include "Vector3D.h"

namespace Retoccilus::Engine3D::ModelImporter {
    template <typename Tp> class RtMIMatrix3x3_Template;
    template <typename Tp> class RtMIQuaternion_Template;

    template <typename Tp> class RtMIMatrix4x4_Template {
    public:
        RtMIMatrix4x4_Template() noexcept;

        RtMIMatrix4x4_Template(Tp _a1, Tp _a2, Tp _a3, Tp _a4, Tp _b1, Tp _b2, Tp _b3, Tp _b4, Tp _c1, Tp _c2, Tp _c3,
                               Tp _c4, Tp _d1, Tp _d2, Tp _d3, Tp _d4);

        explicit RtMIMatrix4x4_Template(const RtMIMatrix3x3_Template<Tp> &m);

        RtMIMatrix4x4_Template(const RtMIVector3D_Template<Tp> &scaling, const RtMIQuaternion_Template<Tp> &rotation,
                               const RtMIVector3D_Template<Tp> &position);

        Tp *operator[](unsigned int p_iIndex);

        const Tp *operator[](unsigned int p_iIndex) const;

        bool operator==(const RtMIMatrix4x4_Template &m) const;
        bool operator!=(const RtMIMatrix4x4_Template &m) const;

        bool equal(const RtMIMatrix4x4_Template &m, Tp epsilon = g_epsilon) const;

        RtMIMatrix4x4_Template &operator*=(const RtMIMatrix4x4_Template &m);
        RtMIMatrix4x4_Template operator*(const RtMIMatrix4x4_Template &m) const;
        RtMIMatrix4x4_Template operator*(const Tp &aFloat) const;
        RtMIMatrix4x4_Template operator+(const RtMIMatrix4x4_Template &aMatrix) const;

        template <typename TOther> explicit operator RtMIMatrix4x4_Template<TOther>() const;

        RtMIMatrix4x4_Template &transpose();

        RtMIMatrix4x4_Template &inverse();
        Tp Determinant() const;

        inline bool isIdentity(Tp epsilon = 10e-3f) const;

        void decompose(RtMIVector3D_Template<Tp> &scaling, RtMIQuaternion_Template<Tp> &rotation,
                       RtMIVector3D_Template<Tp> &position) const;

        void decompose(RtMIVector3D_Template<Tp> &pScaling, RtMIVector3D_Template<Tp> &pRotation,
                       RtMIVector3D_Template<Tp> &pPosition) const;

        void decompose(RtMIVector3D_Template<Tp> &pScaling, RtMIVector3D_Template<Tp> &pRotationAxis,
                       Tp &pRotationAngle, RtMIVector3D_Template<Tp> &pPosition) const;

        void decomposeWithNoScaling(RtMIQuaternion_Template<Tp> &rotation, RtMIVector3D_Template<Tp> &position) const;

        RtMIMatrix4x4_Template &fromEulerAnglesXYZ(Tp x, Tp y, Tp z);
        RtMIMatrix4x4_Template &fromEulerAnglesXYZ(const RtMIVector3D_Template<Tp> &blubb);

        static RtMIMatrix4x4_Template &rotationX(Tp a, RtMIMatrix4x4_Template &out);

        static RtMIMatrix4x4_Template &rotationY(Tp a, RtMIMatrix4x4_Template &out);

        static RtMIMatrix4x4_Template &rotationZ(Tp a, RtMIMatrix4x4_Template &out);

        static RtMIMatrix4x4_Template &rotation(Tp a, const RtMIVector3D_Template<Tp> &axis,
                                                RtMIMatrix4x4_Template &out);

        static RtMIMatrix4x4_Template &translation(const RtMIVector3D_Template<Tp> &v, RtMIMatrix4x4_Template &out);

        static RtMIMatrix4x4_Template &scaling(const RtMIVector3D_Template<Tp> &v, RtMIMatrix4x4_Template &out);

        static RtMIMatrix4x4_Template &fromMatrixToMatrix(const RtMIVector3D_Template<Tp> &from,
                                                          const RtMIVector3D_Template<Tp> &to,
                                                          RtMIMatrix4x4_Template &out);

        Tp a1, a2, a3, a4;
        Tp b1, b2, b3, b4;
        Tp c1, c2, c3, c4;
        Tp d1, d2, d3, d4;
    };
} // namespace Retoccilus::Engine3D::ModelImporter

#endif