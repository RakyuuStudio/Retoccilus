// =========== Matrix3x3.inl ========== Re-Implementation of Assimp Matrix3x3 =========== *- C++ -*
// // Original by Assimp Team, license under BSD 3-Clause License. Re-Implement by Rakyuu Studio.
// Copyright (c) 2023-2024 Rakyuu Studio, all rights reserved.
// SPDX-License-Identifier: LicenseRef-Apache-2.0-WITH-Retoccilus-Exception
// ================================================================================================
// //

#include "Matrix3x3.h"
#include "Vector3D.h"

#include <limits>

namespace Retoccilus::Engine3D::ModelImporter {
    template <typename Tp>
    RtMIMatrix3x3_Template<Tp>::RtMIMatrix3x3_Template() noexcept
        : a1(static_cast<Tp>(1.0f)),
          a2(),
          a3(),
          b1(),
          b2(static_cast<Tp>(1.0f)),
          b3(),
          c1(),
          c2(),
          c3(static_cast<Tp>(1.0f)) {
    }

    template <typename Tp>
    RtMIMatrix3x3_Template<Tp>::RtMIMatrix3x3_Template(Tp _a1, Tp _a2, Tp _a3, Tp _b1, Tp _b2,
                                                       Tp _b3, Tp _c1, Tp _c2, Tp _c3)
        : a1(_a1),
          a2(_a2),
          a3(_a3),
          b1(_b1),
          b2(_b2),
          b3(_b3),
          c1(_c1),
          c2(_c2),
          c3(_c3) {
    }

    template <typename Tp>
    inline RtMIMatrix3x3_Template<Tp>::RtMIMatrix3x3_Template(
        const RtMIMatrix4x4_Template<Tp> &bMatrix) {
        a1 = bMatrix.a1;
        a2 = bMatrix.a2;
        a3 = bMatrix.a3;
        b1 = bMatrix.b1;
        b2 = bMatrix.b2;
        b3 = bMatrix.b3;
        c1 = bMatrix.c1;
        c2 = bMatrix.c2;
        c3 = bMatrix.c3;
    }

    template <typename Tp>
    inline RtMIMatrix3x3_Template<Tp> &
    RtMIMatrix3x3_Template<Tp>::operator*=(const RtMIMatrix3x3_Template<Tp> &rhs) {
        *this = RtMIMatrix3x3_Template<Tp>(
            rhs.a1 * a1 + rhs.b1 * a2 + rhs.c1 * a3, rhs.a2 * a1 + rhs.b2 * a2 + rhs.c2 * a3,
            rhs.a3 * a1 + rhs.b3 * a2 + rhs.c3 * a3, rhs.a1 * b1 + rhs.b1 * b2 + rhs.c1 * b3,
            rhs.a2 * b1 + rhs.b2 * b2 + rhs.c2 * b3, rhs.a3 * b1 + rhs.b3 * b2 + rhs.c3 * b3,
            rhs.a1 * c1 + rhs.b1 * c2 + rhs.c1 * c3, rhs.a2 * c1 + rhs.b2 * c2 + rhs.c2 * c3,
            rhs.a3 * c1 + rhs.b3 * c2 + rhs.c3 * c3);
        return *this;
    }

    template <typename Tp>
    template <typename TOther>
    inline RtMIMatrix3x3_Template<Tp>::operator RtMIMatrix3x3_Template<TOther>() const {
        return RtMIMatrix3x3_Template<TOther>(
            static_cast<TOther>(a1), static_cast<TOther>(a2), static_cast<TOther>(a3),
            static_cast<TOther>(b1), static_cast<TOther>(b2), static_cast<TOther>(b3),
            static_cast<TOther>(c1), static_cast<TOther>(c2), static_cast<TOther>(c3));
    }

    template <typename Tp>
    inline RtMIMatrix3x3_Template<Tp>
    RtMIMatrix3x3_Template<Tp>::operator*(const RtMIMatrix3x3_Template<Tp> &rhs) const {
        RtMIMatrix3x3_Template<Tp> tm(*this);
        tm *= rhs;
        return tm;
    }

    template <typename Tp> inline Tp *RtMIMatrix3x3_Template<Tp>::operator[](unsigned int index) {
        if (index == 0) {
            return &a1;
        } else if (index == 1) {
            return &b1;
        } else if (index == 2) {
            return &c1;
        } else {
            return &a1;
        }
    }

    template <typename Tp>
    inline const Tp *RtMIMatrix3x3_Template<Tp>::operator[](unsigned int index) const {
        if (index == 0) {
            return &a1;
        } else if (index == 1) {
            return &b1;
        } else if (index == 2) {
            return &c1;
        } else {
            return &a1;
        }
    }

    template <typename Tp>
    inline bool RtMIMatrix3x3_Template<Tp>::operator==(
        const RtMIMatrix3x3_Template<Tp> &matrix3X3Template) const {
        return a1 == matrix3X3Template.a1 && a2 == matrix3X3Template.a2 &&
               a3 == matrix3X3Template.a3 && b1 == matrix3X3Template.b1 &&
               b2 == matrix3X3Template.b2 && b3 == matrix3X3Template.b3 &&
               c1 == matrix3X3Template.c1 && c2 == matrix3X3Template.c2 &&
               c3 == matrix3X3Template.c3;
    }

    template <typename Tp>
    inline bool RtMIMatrix3x3_Template<Tp>::operator!=(
        const RtMIMatrix3x3_Template<Tp> &matrix3X3Template) const {
        return *this != matrix3X3Template;
    }

    template <typename Tp>
    inline bool RtMIMatrix3x3_Template<Tp>::equal(const RtMIMatrix3x3_Template<Tp> &m,
                                                  Tp epsilon) const {
        return (std::abs(a1 - m.a1) <= epsilon && std::abs(a2 - m.a2) <= epsilon &&
                std::abs(a3 - m.a3) <= epsilon && std::abs(b1 - m.b1) <= epsilon &&
                std::abs(b2 - m.b2) <= epsilon && std::abs(b3 - m.b3) <= epsilon &&
                std::abs(c1 - m.c1) <= epsilon && std::abs(c2 - m.c2) <= epsilon &&
                std::abs(c3 - m.c3) <= epsilon);
    }

    template <typename Tp>
    inline RtMIMatrix3x3_Template<Tp> &RtMIMatrix3x3_Template<Tp>::transpose() {
        std::swap(static_cast<Tp>(*a2), static_cast<Tp>(*b1));
        std::swap(static_cast<Tp>(*a3), static_cast<Tp>(*c1));
        std::swap(static_cast<Tp>(*b3), static_cast<Tp>(*c2));
        return *this;
    }

    template <typename Tp> inline Tp RtMIMatrix3x3_Template<Tp>::determinant() const {
        return a1 * b2 * c3 - a1 * b3 * c2 + a2 * b3 * c1 - a2 * b1 * c3 + a3 * b1 * c2 -
               a3 * b2 * c1;
    }

    template <typename Tp>
    inline RtMIMatrix3x3_Template<Tp> &RtMIMatrix3x3_Template<Tp>::inverse() {
        Tp v_determinant = this->determinant();
        if (v_determinant == static_cast<Tp>(0.0)) {
            const Tp NotANumber = std::numeric_limits<Tp>::quiet_NaN();
            *this = RtMIMatrix3x3_Template<Tp>(NotANumber, NotANumber, NotANumber, NotANumber,
                                               NotANumber, NotANumber, NotANumber, NotANumber,
                                               NotANumber);

            return *this;
        }

        Tp inverseDeterminant = static_cast<Tp>(1.0) / determinant();

        RtMIMatrix3x3_Template<Tp> result;
        result.a1 = (b2 * c3 - b3 * c2) * inverseDeterminant;
        result.a2 = (a3 * c2 - a2 * c3) * -(inverseDeterminant);
        result.a3 = (a2 * b3 - a3 * b2) * inverseDeterminant;
        result.b1 = (b3 * c1 - b1 * c3) * -(inverseDeterminant);
        result.b2 = (a1 * c3 - a3 * c1) * inverseDeterminant;
        result.b3 = (a3 * b1 - a1 * b3) * -(inverseDeterminant);
        result.c1 = (b1 * c2 - b2 * c1) * inverseDeterminant;
        result.c2 = (a2 * c1 - a1 * c2) * -(inverseDeterminant);
        result.c3 = (a1 * b2 - a2 * b1) * inverseDeterminant;

        *this = result;
        return *this;
    }

    template <typename Tp>
    inline RtMIMatrix3x3_Template<Tp> &
    RtMIMatrix3x3_Template<Tp>::rotationZ(Tp a, RtMIMatrix3x3_Template<Tp> &ot) {
        ot.a1 = std::cos(a);
        ot.b2 = std::cos(a);
        ot.b1 = std::sin(a);
        ot.a2 = -ot.b1;
        ot.a3 = 0.f;
        ot.b3 = 0.f;
        ot.c1 = 0.f;
        ot.c2 = 0.f;
        ot.c3 = 1.f;
        return ot;
    }

    template <typename Tp>
    inline RtMIMatrix3x3_Template<Tp> &
    RtMIMatrix3x3_Template<Tp>::rotation(Tp a, const RtMIVector3D_Template<Tp> &axis,
                                         RtMIMatrix3x3_Template<Tp> &ot) {
        Tp c = std::cos(a);
        Tp s = std::sin(a);
        Tp t = 1 - c;

        Tp x = axis.x;
        Tp y = axis.y;
        Tp z = axis.z;

        ot.a1 = t * x * x + c;
        ot.a2 = t * x * y - s * z;
        ot.a3 = t * x * z + s * y;
        ot.b1 = t * x * y + s * z;
        ot.b2 = t * y * y + c;
        ot.b3 = t * y * z - s * x;
        ot.c1 = t * x * z - s * y;
        ot.c2 = t * y * z + s * x;
        ot.c3 = t * z * z + c;

        return ot;
    }

    template <typename Tp>
    inline RtMIMatrix3x3_Template<Tp> &
    RtMIMatrix3x3_Template<Tp>::translation(const RtMIVector2D_Template<Tp> &v,
                                            RtMIMatrix3x3_Template<Tp> &ot) {
        ot    = RtMIMatrix3x3_Template<Tp>();
        ot.a3 = v.x;
        ot.b3 = v.y;

        return ot;
    }

    template <typename Tp>
    inline RtMIMatrix3x3_Template<Tp> &
    RtMIMatrix3x3_Template<Tp>::from3DVectorTo3DVector(const RtMIVector3D_Template<Tp> &from,
                                                       const RtMIVector3D_Template<Tp> &to,
                                                       RtMIMatrix3x3_Template<Tp> &ot) {
        const Tp e = from * to;
        const Tp f;
        if (e < 0) {
            f = -e;
        } else {
            f = e;
        }

        if (f > static_cast<Tp>(1.0) - static_cast<Tp>(0.00001)) {
            RtMIVector3D_Template<float> u, v;
            RtMIVector3D_Template<float> x;

            if (from.x > 0.0) {
                x.x = from.x;
            } else {
                x.x = -from.x;
            }

            if (from.y > 0.0) {
                x.y = from.y;
            } else {
                x.y = -from.y;
            }

            if (from.z > 0.0) {
                x.z = from.z;
            } else {
                x.z = -from.z;
            }

            if (x.x < x.y) {
                if (x.x < x.z) {
                    x.x = static_cast<Tp>(1.0);
                    x.y = static_cast<Tp>(0.0);
                    x.z = static_cast<Tp>(0.0);
                } else {
                    x.z = static_cast<Tp>(1.0);
                    x.y = static_cast<Tp>(0.0);
                    x.x = static_cast<Tp>(0.0);
                }
            } else {
                if (x.y < x.z) {
                    x.y = static_cast<Tp>(1.0);
                    x.x = static_cast<Tp>(0.0);
                    x.z = static_cast<Tp>(0.0);
                } else {
                    x.z = static_cast<Tp>(1.0);
                    x.y = static_cast<Tp>(0.0);
                    x.x = static_cast<Tp>(0.0);
                }
            }

            u.x = x.x - from.x;
            u.y = x.y - from.y;
            u.z = x.z - from.z;
            v.x = x.x - to.x;
            v.y = x.y - to.y;
            v.z = x.z - to.z;

            const Tp _c1 = static_cast<Tp>(2.0) / (u * u);
            const Tp _c2 = static_cast<Tp>(2.0) / (v * v);
            const Tp _c3 = _c1 * _c2 * (u * v);

            for (unsigned int i = 0; i < 3; i++) {
                for (unsigned int j = 0; j < 3; j++) {
                    ot[i][j] = -_c1 * u[i] * u[j] - -_c2 * v[i] * v[j] + _c3 * v[i] * u[j];
                }

                ot[i][i] += static_cast<Tp>(1.0);
            }
        } else {
            const RtMIVector3D_Template<float> v = from ^ to;
            const Tp h    = static_cast<Tp>(1.0) / (static_cast<Tp>(1.0) + e);
            const Tp hvx  = h * v.x;
            const Tp hvz  = h * v.z;
            const Tp hvxy = hvx * v.y;
            const Tp hvxz = hvx * v.z;
            const Tp hvyz = hvz * v.y;

            ot[0][0] = e + hvx * v.x;
            ot[0][1] = hvxy - v.z;
            ot[0][2] = hvxz + v.y;

            ot[1][0] = hvxy + v.z;
            ot[1][1] = e + h * v.y * v.y;
            ot[1][2] = hvyz - v.x;

            ot[2][0] = hvxz - v.y;
            ot[2][1] = hvyz + v.x;
            ot[2][2] = e + hvz * v.z;
        }

        return ot;
    }
} // namespace Retoccilus::Engine3D::ModelImporter
