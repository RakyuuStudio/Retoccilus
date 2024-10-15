// =========== Matrix4x4.inl ========== Re-Implementation of Assimp Matrix4x4 =========== *- C++ -*
// // Original by Assimp Team, license under BSD 3-Clause License. Re-Implement by Rakyuu Studio.
// Copyright (c) 2023-2024 Rakyuu Studio, all rights reserved.
// SPDX-License-Identifier: LicenseRef-Apache-2.0-WITH-Retoccilus-Exception
// ================================================================================================
// //

#include "Matrix4x4.h"
#include <cmath>
#include <limits>

namespace Retoccilus::Engine3D::ModelImporter {
    template <typename Tp>
    RtMIMatrix4x4_Template<Tp>::RtMIMatrix4x4_Template() noexcept
        : a1(1.0f),
          a2(),
          a3(),
          a4(),
          b1(),
          b2(1.0f),
          b3(),
          b4(),
          c1(),
          c2(),
          c3(1.0f),
          c4(),
          d1(),
          d2(),
          d3(),
          d4(1.0f) {
    }

    template <typename Tp>
    RtMIMatrix4x4_Template<Tp>::RtMIMatrix4x4_Template(Tp _a1, Tp _a2, Tp _a3, Tp _a4, Tp _b1,
                                                       Tp _b2, Tp _b3, Tp _b4, Tp _c1, Tp _c2,
                                                       Tp _c3, Tp _c4, Tp _d1, Tp _d2, Tp _d3,
                                                       Tp _d4)
        : a1(_a1),
          a2(_a2),
          a3(_a3),
          a4(_a4),
          b1(_b1),
          b2(_b2),
          b3(_b3),
          b4(_b4),
          c1(_c1),
          c2(_c2),
          c3(_c3),
          c4(_c4),
          d1(_d1),
          d2(_d2),
          d3(_d3),
          d4(_d4) {
    }

    template <typename Tp>
    template <typename TOther>
    RtMIMatrix4x4_Template<Tp>::operator RtMIMatrix4x4_Template<TOther>() const {
        return RtMIMatrix4x4_Template<TOther>(
            static_cast<TOther>(a1), static_cast<TOther>(a2), static_cast<TOther>(a3),
            static_cast<TOther>(a4), static_cast<TOther>(b1), static_cast<TOther>(b2),
            static_cast<TOther>(b3), static_cast<TOther>(b4), static_cast<TOther>(c1),
            static_cast<TOther>(c2), static_cast<TOther>(c3), static_cast<TOther>(c4),
            static_cast<TOther>(d1), static_cast<TOther>(d2), static_cast<TOther>(d3),
            static_cast<TOther>(d4));
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp>::RtMIMatrix4x4_Template(const RtMIMatrix3x3_Template<Tp> &m) {
        a1 = m.a1;
        a2 = m.a2;
        a3 = m.a3;
        a4 = static_cast<Tp>(0.0);
        b1 = m.b1;
        b2 = m.b2;
        b3 = m.b3;
        b4 = static_cast<Tp>(0.0);
        c1 = m.c1;
        c2 = m.c2;
        c3 = m.c3;
        c4 = static_cast<Tp>(0.0);
        d1 = static_cast<Tp>(0.0);
        d2 = static_cast<Tp>(0.0);
        d3 = static_cast<Tp>(0.0);
        d4 = static_cast<Tp>(1.0);
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp>::RtMIMatrix4x4_Template(
        const RtMIVector3D_Template<Tp> &scaling, const RtMIQuaternion_Template<Tp> &rotation,
        const RtMIVector3D_Template<Tp> &position) {
        RtMIMatrix3x3_Template<Tp> m = rotation.GetMatrix();

        a1 = m.a1 * scaling.x;
        a2 = m.a2 * scaling.x;
        a3 = m.a3 * scaling.x;
        a4 = position.x;

        b1 = m.b1 * scaling.y;
        b2 = m.b2 * scaling.y;
        b3 = m.b3 * scaling.y;
        b4 = position.y;

        c1 = m.c1 * scaling.z;
        c2 = m.c2 * scaling.z;
        c3 = m.c3 * scaling.z;
        c4 = position.z;

        d1 = static_cast<Tp>(0.0);
        d2 = static_cast<Tp>(0.0);
        d3 = static_cast<Tp>(0.0);
        d4 = static_cast<Tp>(1.0);
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp> &
    RtMIMatrix4x4_Template<Tp>::operator*=(const RtMIMatrix4x4_Template<Tp> &m) {
        *this = RtMIMatrix4x4_Template<Tp>(m.a1 * a1 + m.b1 * a2 + m.c1 * a3 + m.d1 * a4,
                                           m.a2 * a1 + m.b2 * a2 + m.c2 * a3 + m.d2 * a4,
                                           m.a3 * a1 + m.b3 * a2 + m.c3 * a3 + m.d3 * a4,
                                           m.a4 * a1 + m.b4 * a2 + m.c4 * a3 + m.d4 * a4,
                                           m.a1 * b1 + m.b1 * b2 + m.c1 * b3 + m.d1 * b4,
                                           m.a2 * b1 + m.b2 * b2 + m.c2 * b3 + m.d2 * b4,
                                           m.a3 * b1 + m.b3 * b2 + m.c3 * b3 + m.d3 * b4,
                                           m.a4 * b1 + m.b4 * b2 + m.c4 * b3 + m.d4 * b4,
                                           m.a1 * c1 + m.b1 * c2 + m.c1 * c3 + m.d1 * c4,
                                           m.a2 * c1 + m.b2 * c2 + m.c2 * c3 + m.d2 * c4,
                                           m.a3 * c1 + m.b3 * c2 + m.c3 * c3 + m.d3 * c4,
                                           m.a4 * c1 + m.b4 * c2 + m.c4 * c3 + m.d4 * c4,
                                           m.a1 * d1 + m.b1 * d2 + m.c1 * d3 + m.d1 * d4,
                                           m.a2 * d1 + m.b2 * d2 + m.c2 * d3 + m.d2 * d4,
                                           m.a3 * d1 + m.b3 * d2 + m.c3 * d3 + m.d3 * d4,
                                           m.a4 * d1 + m.b4 * d2 + m.c4 * d3 + m.d4 * d4);
        return *this;
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp>
    RtMIMatrix4x4_Template<Tp>::operator*(const Tp &aFloat) const {
        RtMIMatrix4x4_Template<Tp> temp(a1 * aFloat, a2 * aFloat, a3 * aFloat, a4 * aFloat,
                                        b1 * aFloat, b2 * aFloat, b3 * aFloat, b4 * aFloat,
                                        c1 * aFloat, c2 * aFloat, c3 * aFloat, c4 * aFloat,
                                        d1 * aFloat, d2 * aFloat, d3 * aFloat, d4 * aFloat);
        return temp;
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp>
    RtMIMatrix4x4_Template<Tp>::operator+(const RtMIMatrix4x4_Template<Tp> &m) const {
        RtMIMatrix4x4_Template<Tp> temp(
            m.a1 + a1, m.a2 + a2, m.a3 + a3, m.a4 + a4, m.b1 + b1, m.b2 + b2, m.b3 + b3, m.b4 + b4,
            m.c1 + c1, m.c2 + c2, m.c3 + c3, m.c4 + c4, m.d1 + d1, m.d2 + d2, m.d3 + d3, m.d4 + d4);
        return temp;
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp>
    RtMIMatrix4x4_Template<Tp>::operator*(const RtMIMatrix4x4_Template<Tp> &m) const {
        RtMIMatrix4x4_Template<Tp> temp(*this);
        temp *= m;
        return temp;
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp> &RtMIMatrix4x4_Template<Tp>::transpose() {
        std::swap((Tp &)b1, (Tp &)a2);
        std::swap((Tp &)c1, (Tp &)a3);
        std::swap((Tp &)c2, (Tp &)b3);
        std::swap((Tp &)d1, (Tp &)a4);
        std::swap((Tp &)d2, (Tp &)b4);
        std::swap((Tp &)d3, (Tp &)c4);
        return *this;
    }

    template <typename Tp> inline Tp RtMIMatrix4x4_Template<Tp>::determinant() const {
        return a1 * b2 * c3 * d4 - a1 * b2 * c4 * d3 + a1 * b3 * c4 * d2 - a1 * b3 * c2 * d4 +
               a1 * b4 * c2 * d3 - a1 * b4 * c3 * d2 - a2 * b3 * c4 * d1 + a2 * b3 * c1 * d4 -
               a2 * b4 * c1 * d3 + a2 * b4 * c3 * d1 - a2 * b1 * c3 * d4 + a2 * b1 * c4 * d3 +
               a3 * b4 * c1 * d2 - a3 * b4 * c2 * d1 + a3 * b1 * c2 * d4 - a3 * b1 * c4 * d2 +
               a3 * b2 * c4 * d1 - a3 * b2 * c1 * d4 - a4 * b1 * c2 * d3 + a4 * b1 * c3 * d2 -
               a4 * b2 * c3 * d1 + a4 * b2 * c1 * d3 - a4 * b3 * c1 * d2 + a4 * b3 * c2 * d1;
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp> &RtMIMatrix4x4_Template<Tp>::inverse() {
        const Tp det = this->determinant();
        if (det == static_cast<Tp>(0.0)) {
            const Tp nan = std::numeric_limits<Tp>::quiet_NaN();
            *this = RtMIMatrix4x4_Template<Tp>(nan, nan, nan, nan, nan, nan, nan, nan, nan, nan,
                                               nan, nan, nan, nan, nan, nan);

            return *this;
        }

        const Tp inversedDeterminant = static_cast<Tp>(1.0) / det;

        RtMIMatrix4x4_Template<Tp> res;
        res.a1 = inversedDeterminant *
                 (b2 * (c3 * d4 - c4 * d3) + b3 * (c4 * d2 - c2 * d4) + b4 * (c2 * d3 - c3 * d2));
        res.a2 = -inversedDeterminant *
                 (a2 * (c3 * d4 - c4 * d3) + a3 * (c4 * d2 - c2 * d4) + a4 * (c2 * d3 - c3 * d2));
        res.a3 = inversedDeterminant *
                 (a2 * (b3 * d4 - b4 * d3) + a3 * (b4 * d2 - b2 * d4) + a4 * (b2 * d3 - b3 * d2));
        res.a4 = -inversedDeterminant *
                 (a2 * (b3 * c4 - b4 * c3) + a3 * (b4 * c2 - b2 * c4) + a4 * (b2 * c3 - b3 * c2));
        res.b1 = -inversedDeterminant *
                 (b1 * (c3 * d4 - c4 * d3) + b3 * (c4 * d1 - c1 * d4) + b4 * (c1 * d3 - c3 * d1));
        res.b2 = inversedDeterminant *
                 (a1 * (c3 * d4 - c4 * d3) + a3 * (c4 * d1 - c1 * d4) + a4 * (c1 * d3 - c3 * d1));
        res.b3 = -inversedDeterminant *
                 (a1 * (b3 * d4 - b4 * d3) + a3 * (b4 * d1 - b1 * d4) + a4 * (b1 * d3 - b3 * d1));
        res.b4 = inversedDeterminant *
                 (a1 * (b3 * c4 - b4 * c3) + a3 * (b4 * c1 - b1 * c4) + a4 * (b1 * c3 - b3 * c1));
        res.c1 = inversedDeterminant *
                 (b1 * (c2 * d4 - c4 * d2) + b2 * (c4 * d1 - c1 * d4) + b4 * (c1 * d2 - c2 * d1));
        res.c2 = -inversedDeterminant *
                 (a1 * (c2 * d4 - c4 * d2) + a2 * (c4 * d1 - c1 * d4) + a4 * (c1 * d2 - c2 * d1));
        res.c3 = inversedDeterminant *
                 (a1 * (b2 * d4 - b4 * d2) + a2 * (b4 * d1 - b1 * d4) + a4 * (b1 * d2 - b2 * d1));
        res.c4 = -inversedDeterminant *
                 (a1 * (b2 * c4 - b4 * c2) + a2 * (b4 * c1 - b1 * c4) + a4 * (b1 * c2 - b2 * c1));
        res.d1 = -inversedDeterminant *
                 (b1 * (c2 * d3 - c3 * d2) + b2 * (c3 * d1 - c1 * d3) + b3 * (c1 * d2 - c2 * d1));
        res.d2 = inversedDeterminant *
                 (a1 * (c2 * d3 - c3 * d2) + a2 * (c3 * d1 - c1 * d3) + a3 * (c1 * d2 - c2 * d1));
        res.d3 = -inversedDeterminant *
                 (a1 * (b2 * d3 - b3 * d2) + a2 * (b3 * d1 - b1 * d3) + a3 * (b1 * d2 - b2 * d1));
        res.d4 = inversedDeterminant *
                 (a1 * (b2 * c3 - b3 * c2) + a2 * (b3 * c1 - b1 * c3) + a3 * (b1 * c2 - b2 * c1));
        *this = res;

        return *this;
    }

    template <typename Tp>
    inline Tp *RtMIMatrix4x4_Template<Tp>::operator[](unsigned int p_iIndex) {
        if (p_iIndex > 3) {
            return nullptr;
        }
        switch (p_iIndex) {
        case 0:
            return &a1;
        case 1:
            return &b1;
        case 2:
            return &c1;
        case 3:
            return &d1;
        default:
            break;
        }
        return &a1;
    }

    template <typename Tp>
    inline const Tp *RtMIMatrix4x4_Template<Tp>::operator[](unsigned int p_iIndex) const {
        if (p_iIndex > 3) {
            return nullptr;
        }

        switch (p_iIndex) {
        case 0:
            return &a1;
        case 1:
            return &b1;
        case 2:
            return &c1;
        case 3:
            return &d1;
        default:
            break;
        }
        return &a1;
    }

    template <typename Tp>
    inline bool RtMIMatrix4x4_Template<Tp>::operator==(const RtMIMatrix4x4_Template<Tp> &m) const {
        return (a1 == m.a1 && a2 == m.a2 && a3 == m.a3 && a4 == m.a4 && b1 == m.b1 && b2 == m.b2 &&
                b3 == m.b3 && b4 == m.b4 && c1 == m.c1 && c2 == m.c2 && c3 == m.c3 && c4 == m.c4 &&
                d1 == m.d1 && d2 == m.d2 && d3 == m.d3 && d4 == m.d4);
    }

    template <typename Tp>
    inline bool RtMIMatrix4x4_Template<Tp>::operator!=(const RtMIMatrix4x4_Template<Tp> &m) const {
        return *this != m;
    }

    template <typename Tp>
    inline bool RtMIMatrix4x4_Template<Tp>::equal(const RtMIMatrix4x4_Template &m,
                                                  Tp epsilon) const {
        return std::abs(a1 - m.a1) <= epsilon && std::abs(a2 - m.a2) <= epsilon &&
               std::abs(a3 - m.a3) <= epsilon && std::abs(a4 - m.a4) <= epsilon &&
               std::abs(b1 - m.b1) <= epsilon && std::abs(b2 - m.b2) <= epsilon &&
               std::abs(b3 - m.b3) <= epsilon && std::abs(b4 - m.b4) <= epsilon &&
               std::abs(c1 - m.c1) <= epsilon && std::abs(c2 - m.c2) <= epsilon &&
               std::abs(c3 - m.c3) <= epsilon && std::abs(c4 - m.c4) <= epsilon &&
               std::abs(d1 - m.d1) <= epsilon && std::abs(d2 - m.d2) <= epsilon &&
               std::abs(d3 - m.d3) <= epsilon && std::abs(d4 - m.d4) <= epsilon;
    }

    //#define R_DECOMPOSE_MATRIX                                                                                             \
//    const RtMIMatrix4x4_Template<Tp> &_this = *this;                                                                   \
//                                                                                                                       \
//    pPosition.x = _this[0][3];                                                                                         \
//    pPosition.y = _this[1][3];                                                                                         \
//    pPosition.z = _this[2][3];                                                                                         \
//                                                                                                                       \
//    RtMIVector3D_Template<Tp> vCols[3] = {RtMIVector3D_Template<Tp>(_this[0][0], _this[1][0], _this[2][0]),            \
//                                          RtMIVector3D_Template<Tp>(_this[0][1], _this[1][1], _this[2][1]),            \
//                                          RtMIVector3D_Template<Tp>(_this[0][2], _this[1][2], _this[2][2])};           \
//                                                                                                                       \
//    pScaling.x = vCols[0].Length();                                                                                    \
//    pScaling.y = vCols[1].Length();                                                                                    \
//    pScaling.z = vCols[2].Length();                                                                                    \
//                                                                                                                       \
//    if (this->determinant() < 0)                                                                                       \
//        pScaling = -pScaling;                                                                                          \
//                                                                                                                       \
//    if (pScaling.x)                                                                                                    \
//        vCols[0] /= pScaling.x;                                                                                        \
//    if (pScaling.y)                                                                                                    \
//        vCols[1] /= pScaling.y;                                                                                        \
//    if (pScaling.z)                                                                                                    \
//        vCols[2] /= pScaling.z;                                                                                        \
//                                                                                                                       \
//    do {                                                                                                               \
//    } while (false)

    template <typename Tp>
    inline void RtMIMatrix4x4_Template<Tp>::decompose(RtMIVector3D_Template<Tp> &pScaling,
                                                      RtMIQuaternion_Template<Tp> &pRotation,
                                                      RtMIVector3D_Template<Tp> &pPosition) const {
        decomposeMatrix(pPosition, pScaling);

        RtMIMatrix3x3_Template<Tp> m(vCols[0].x, vCols[1].x, vCols[2].x, vCols[0].y, vCols[1].y,
                                     vCols[2].y, vCols[0].z, vCols[1].z, vCols[2].z);
        pRotation = RtMIQuaternion_Template<Tp>(m);
    }

    template <typename Tp>
    inline void RtMIMatrix4x4_Template<Tp>::decompose(RtMIVector3D_Template<Tp> &pScaling,
                                                      RtMIVector3D_Template<Tp> &pRotation,
                                                      RtMIVector3D_Template<Tp> &pPosition) const {
        decomposeMatrix(pPosition, pScaling);
        const Tp epsilon = static_cast<Tp>(std::numeric_limits<Tp>::epsilon());

        pRotation.y = std::asin(-vCols[0].z); // D. Angle around oY.

        Tp C = std::cos(pRotation.y);

        if (std::fabs(C) > epsilon) {
            Tp tan_x = vCols[2].z / C;
            Tp tan_y = vCols[1].z / C;

            pRotation.x = std::atan2(tan_y, tan_x);
            tan_x       = vCols[0].x / C;
            tan_y       = vCols[0].y / C;
            pRotation.z = std::atan2(tan_y, tan_x);
        } else {
            pRotation.x = 0;
            Tp tan_x    = vCols[1].y;
            Tp tan_y    = -vCols[1].x;

            pRotation.z = std::atan2(tan_y, tan_x);
        }
    }

#undef R_DECOMPOSE_MATRIX

    template <typename Tp>
    inline void RtMIMatrix4x4_Template<Tp>::decompose(RtMIVector3D_Template<Tp> &pScaling,
                                                      RtMIVector3D_Template<Tp> &pRotationAxis,
                                                      Tp &pRotationAngle,
                                                      RtMIVector3D_Template<Tp> &pPosition) const {
        RtMIQuaternion_Template<Tp> pRotation;

        decompose(pScaling, pRotation, pPosition);
        pRotation.Normalize();

        Tp angle_cos = pRotation.w;
        Tp angle_sin = std::sqrt(1.0f - angle_cos * angle_cos);

        pRotationAngle   = std::acos(angle_cos) * 2;
        const Tp epsilon = 10e-3f;

        if (std::fabs(angle_sin) < epsilon)
            angle_sin = 1;

        pRotationAxis.x = pRotation.x / angle_sin;
        pRotationAxis.y = pRotation.y / angle_sin;
        pRotationAxis.z = pRotation.z / angle_sin;
    }

    template <typename Tp>
    inline void
    RtMIMatrix4x4_Template<Tp>::decomposeWithNoScaling(RtMIQuaternion_Template<Tp> &rotation,
                                                       RtMIVector3D_Template<Tp> &position) const {
        const RtMIMatrix4x4_Template<Tp> &_this = *this;

        position.x = _this[0][3];
        position.y = _this[1][3];
        position.z = _this[2][3];

        rotation = RtMIQuaternion_Template<Tp>((RtMIMatrix3x3_Template<Tp>)_this);
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp> &
    RtMIMatrix4x4_Template<Tp>::fromEulerAnglesXYZ(const RtMIVector3D_Template<Tp> &blubb) {
        return fromEulerAnglesXYZ(blubb.x, blubb.y, blubb.z);
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp> &RtMIMatrix4x4_Template<Tp>::fromEulerAnglesXYZ(Tp x, Tp y,
                                                                                      Tp z) {
        RtMIMatrix4x4_Template<Tp> &_this = *this;

        Tp cx = std::cos(x);
        Tp sx = std::sin(x);
        Tp cy = std::cos(y);
        Tp sy = std::sin(y);
        Tp cz = std::cos(z);
        Tp sz = std::sin(z);

        _this.a1 = cz * cy;
        _this.a2 = cz * sy * sx - sz * cx;
        _this.a3 = sz * sx + cz * sy * cx;

        _this.b1 = sz * cy;
        _this.b2 = cz * cx + sz * sy * sx;
        _this.b3 = sz * sy * cx - cz * sx;

        _this.c1 = -sy;
        _this.c2 = cy * sx;
        _this.c3 = cy * cx;

        return *this;
    }

    template <typename Tp>
    inline bool RtMIMatrix4x4_Template<Tp>::isIdentity(const Tp epsilon) const {
        return (a2 <= epsilon && a2 >= -epsilon && a3 <= epsilon && a3 >= -epsilon &&
                a4 <= epsilon && a4 >= -epsilon && b1 <= epsilon && b1 >= -epsilon &&
                b3 <= epsilon && b3 >= -epsilon && b4 <= epsilon && b4 >= -epsilon &&
                c1 <= epsilon && c1 >= -epsilon && c2 <= epsilon && c2 >= -epsilon &&
                c4 <= epsilon && c4 >= -epsilon && d1 <= epsilon && d1 >= -epsilon &&
                d2 <= epsilon && d2 >= -epsilon && d3 <= epsilon && d3 >= -epsilon &&
                a1 <= 1.f + epsilon && a1 >= 1.f - epsilon && b2 <= 1.f + epsilon &&
                b2 >= 1.f - epsilon && c3 <= 1.f + epsilon && c3 >= 1.f - epsilon &&
                d4 <= 1.f + epsilon && d4 >= 1.f - epsilon);
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp> &
    RtMIMatrix4x4_Template<Tp>::rotationX(Tp a, RtMIMatrix4x4_Template &out) {
        out    = RtMIMatrix4x4_Template<Tp>();
        out.b2 = out.c3 = std::cos(a);
        out.b3          = -(out.c2 = std::sin(a));
        return out;
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp> &
    RtMIMatrix4x4_Template<Tp>::rotationY(Tp a, RtMIMatrix4x4_Template &out) {
        out    = RtMIMatrix4x4_Template<Tp>();
        out.a1 = out.c3 = std::cos(a);
        out.c1          = -(out.a3 = std::sin(a));
        return out;
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp> &
    RtMIMatrix4x4_Template<Tp>::rotationZ(Tp a, RtMIMatrix4x4_Template &out) {
        out    = RtMIMatrix4x4_Template<Tp>();
        out.a1 = out.b2 = std::cos(a);
        out.a2          = -(out.b1 = std::sin(a));
        return out;
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp> &
    RtMIMatrix4x4_Template<Tp>::rotation(Tp a, const RtMIVector3D_Template<Tp> &axis,
                                         RtMIMatrix4x4_Template &out) {
        Tp c = std::cos(a), s = std::sin(a), t = 1 - c;
        Tp x = axis.x, y = axis.y, z = axis.z;

        out.a1 = t * x * x + c;
        out.a2 = t * x * y - s * z;
        out.a3 = t * x * z + s * y;
        out.b1 = t * x * y + s * z;
        out.b2 = t * y * y + c;
        out.b3 = t * y * z - s * x;
        out.c1 = t * x * z - s * y;
        out.c2 = t * y * z + s * x;
        out.c3 = t * z * z + c;
        out.a4 = out.b4 = out.c4 = static_cast<Tp>(0.0);
        out.d1 = out.d2 = out.d3 = static_cast<Tp>(0.0);
        out.d4                   = static_cast<Tp>(1.0);

        return out;
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp> &
    RtMIMatrix4x4_Template<Tp>::translation(const RtMIVector3D_Template<Tp> &v,
                                            RtMIMatrix4x4_Template &out) {
        out    = RtMIMatrix4x4_Template<Tp>();
        out.a4 = v.x;
        out.b4 = v.y;
        out.c4 = v.z;
        return out;
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp> &
    RtMIMatrix4x4_Template<Tp>::scaling(const RtMIVector3D_Template<Tp> &v,
                                        RtMIMatrix4x4_Template &out) {
        out    = RtMIMatrix4x4_Template<Tp>();
        out.a1 = v.x;
        out.b2 = v.y;
        out.c3 = v.z;
        return out;
    }

    template <typename Tp>
    inline RtMIMatrix4x4_Template<Tp> &
    RtMIMatrix4x4_Template<Tp>::fromMatrixToMatrix(const RtMIVector3D_Template<Tp> &from,
                                                   const RtMIVector3D_Template<Tp> &to,
                                                   RtMIMatrix4x4_Template &mtx) {
        RtMIMatrix3x3_Template<Tp> m3;
        RtMIMatrix3x3_Template<Tp>::FromToMatrix(from, to, m3);
        mtx = RtMIMatrix4x4_Template<Tp>(m3);
        return mtx;
    }
} // namespace Retoccilus::Engine3D::ModelImporter