// =========== Vector3D.inl ========== Re-Implementation of Assimp Vector3D ============= *- C++ -*
// // Original by Assimp Team, license under BSD 3-Clause License. Re-Implement by Rakyuu Studio.
// Copyright (c) 2023-2024 Rakyuu Studio, all rights reserved.
// SPDX-License-Identifier: LicenseRef-Apache-2.0-WITH-Retoccilus-Exception
// ================================================================================================
// //

#include "Vector3D.h"
#include <cmath>

namespace Retoccilus::Engine3D::ModelImporter {
    template <typename Tp>
    RtMIVector3D_Template<Tp>::RtMIVector3D_Template(Tp _xyz) : x(_xyz),
                                                                y(_xyz),
                                                                z(_xyz) {
    }

    template <typename Tp>
    RtMIVector3D_Template<Tp>::RtMIVector3D_Template(Tp _x, Tp _y, Tp _z) : x(_x),
                                                                            y(_y),
                                                                            z(_z) {
    }

    template <typename Tp>
    RtMIVector3D_Template<Tp>::RtMIVector3D_Template() noexcept : x(),
                                                                  y(),
                                                                  z() {
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> operator*(const RtMIMatrix3x3_Template<Tp> &pMatrix,
                                               const RtMIVector3D_Template<Tp> &pVector) {
        RtMIVector3D_Template<Tp> res;
        res.x = pMatrix.a1 * pVector.x + pMatrix.a2 * pVector.y + pMatrix.a3 * pVector.z;
        res.y = pMatrix.b1 * pVector.x + pMatrix.b2 * pVector.y + pMatrix.b3 * pVector.z;
        res.z = pMatrix.c1 * pVector.x + pMatrix.c2 * pVector.y + pMatrix.c3 * pVector.z;
        return res;
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> operator*(const RtMIMatrix4x4_Template<Tp> &pMatrix,
                                               const RtMIVector3D_Template<Tp> &pVector) {
        RtMIVector3D_Template<Tp> res;
        res.x =
            pMatrix.a1 * pVector.x + pMatrix.a2 * pVector.y + pMatrix.a3 * pVector.z + pMatrix.a4;
        res.y =
            pMatrix.b1 * pVector.x + pMatrix.b2 * pVector.y + pMatrix.b3 * pVector.z + pMatrix.b4;
        res.z =
            pMatrix.c1 * pVector.x + pMatrix.c2 * pVector.y + pMatrix.c3 * pVector.z + pMatrix.c4;
        return res;
    }

    template <typename Tp>
    template <typename TOther>
    RtMIVector3D_Template<Tp>::operator RtMIVector3D_Template<TOther>() const {
        return RtMIVector3D_Template<TOther>(static_cast<TOther>(x), static_cast<TOther>(y),
                                             static_cast<TOther>(z));
    }

    template <typename Tp> inline void RtMIVector3D_Template<Tp>::set(Tp pX, Tp pY, Tp pZ) {
        x = pX;
        y = pY;
        z = pZ;
    }

    template <typename Tp> inline Tp RtMIVector3D_Template<Tp>::squareLength() const {
        return x * x + y * y + z * z;
    }

    template <typename Tp> inline Tp RtMIVector3D_Template<Tp>::length() const {
        return std::sqrt(this->squareLength());
    }

    template <typename Tp> RtMIVector3D_Template<Tp> &RtMIVector3D_Template<Tp>::normalize() {
        const Tp l = this->length();
        if (l == 0) {
            return *this;
        }
        *this /= this->length();

        return *this;
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> &RtMIVector3D_Template<Tp>::normalizeSafe() {
        Tp len = this->length();
        if (len > static_cast<Tp>(0)) {
            *this /= len;
        }
        return *this;
    }

    template <typename Tp>
    inline const RtMIVector3D_Template<Tp> &
    RtMIVector3D_Template<Tp>::operator+=(const RtMIVector3D_Template<Tp> &o) {
        x += o.x;
        y += o.y;
        z += o.z;

        return *this;
    }

    template <typename Tp>
    inline const RtMIVector3D_Template<Tp> &
    RtMIVector3D_Template<Tp>::operator-=(const RtMIVector3D_Template<Tp> &o) {
        x -= o.x;
        y -= o.y;
        z -= o.z;

        return *this;
    }

    template <typename Tp>
    inline const RtMIVector3D_Template<Tp> &RtMIVector3D_Template<Tp>::operator*=(Tp f) {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

    template <typename Tp>
    inline const RtMIVector3D_Template<Tp> &RtMIVector3D_Template<Tp>::operator/=(Tp f) {
        if (f == static_cast<Tp>(0.0)) {
            return *this;
        }
        const Tp invF = (Tp)1.0 / f;
        x *= invF;
        y *= invF;
        z *= invF;

        return *this;
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> &
    RtMIVector3D_Template<Tp>::operator*=(const RtMIMatrix3x3_Template<Tp> &mat) {
        return (*this = mat * (*this));
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> &
    RtMIVector3D_Template<Tp>::operator*=(const RtMIMatrix4x4_Template<Tp> &mat) {
        return (*this = mat * (*this));
    }

    template <typename Tp> inline Tp RtMIVector3D_Template<Tp>::operator[](unsigned int i) const {
        switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            break;
        }
        return x;
    }

    template <typename Tp> inline Tp &RtMIVector3D_Template<Tp>::operator[](unsigned int i) {
        switch (i) {
        case 0:
            return x;
        case 1:
            return y;
        case 2:
            return z;
        default:
            break;
        }
        return x;
    }

    template <typename Tp>
    inline bool
    RtMIVector3D_Template<Tp>::operator==(const RtMIVector3D_Template<Tp> &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    template <typename Tp>
    inline bool
    RtMIVector3D_Template<Tp>::operator!=(const RtMIVector3D_Template<Tp> &other) const {
        return x != other.x || y != other.y || z != other.z;
    }

    template <typename Tp>
    inline bool RtMIVector3D_Template<Tp>::equal(const RtMIVector3D_Template<Tp> &other,
                                                 Tp epsilon) const {
        return std::abs(x - other.x) <= epsilon && std::abs(y - other.y) <= epsilon &&
               std::abs(z - other.z) <= epsilon;
    }

    template <typename Tp>
    inline bool RtMIVector3D_Template<Tp>::operator<(const RtMIVector3D_Template<Tp> &other) const {
        return x != other.x ? x < other.x : y != other.y ? y < other.y : z < other.z;
    }

    template <typename Tp>
    inline const RtMIVector3D_Template<Tp>
    RtMIVector3D_Template<Tp>::symmetricMultiply(const RtMIVector3D_Template<Tp> &o) {
        return RtMIVector3D_Template<Tp>(x * o.x, y * o.y, z * o.z);
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> operator+(const RtMIVector3D_Template<Tp> &v1,
                                               const RtMIVector3D_Template<Tp> &v2) {
        return RtMIVector3D_Template<Tp>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> operator-(const RtMIVector3D_Template<Tp> &v1,
                                               const RtMIVector3D_Template<Tp> &v2) {
        return RtMIVector3D_Template<Tp>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    template <typename Tp>
    inline Tp operator*(const RtMIVector3D_Template<Tp> &v1, const RtMIVector3D_Template<Tp> &v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> operator*(Tp f, const RtMIVector3D_Template<Tp> &v) {
        return RtMIVector3D_Template<Tp>(f * v.x, f * v.y, f * v.z);
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> operator*(const RtMIVector3D_Template<Tp> &v, Tp f) {
        return RtMIVector3D_Template<Tp>(f * v.x, f * v.y, f * v.z);
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> operator/(const RtMIVector3D_Template<Tp> &v, Tp f) {
        return v * (1 / f);
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> operator/(const RtMIVector3D_Template<Tp> &v,
                                               const RtMIVector3D_Template<Tp> &v2) {
        return RtMIVector3D_Template<Tp>(v.x / v2.x, v.y / v2.y, v.z / v2.z);
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> operator^(const RtMIVector3D_Template<Tp> &v1,
                                               const RtMIVector3D_Template<Tp> &v2) {
        return RtMIVector3D_Template<Tp>(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z,
                                         v1.x * v2.y - v1.y * v2.x);
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> operator-(const RtMIVector3D_Template<Tp> &v) {
        return RtMIVector3D_Template<Tp>(-v.x, -v.y, -v.z);
    }
} // namespace Retoccilus::Engine3D::ModelImporter