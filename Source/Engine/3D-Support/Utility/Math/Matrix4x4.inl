#include "Matrix4x4.h"
#include "Matrix3x3.h"
#include "Quaternion.h"
#include "../Math.h"

#include <algorithm>
#include <cmath>
#include <limits>

namespace Retoccilus::Engine::ThreeDimensionalSupport::Utility {
    template<typename TemplateReal>
    RtMatrix4x4T<TemplateReal>::RtMatrix4x4T() noexcept :
            a1(1.0f), a2(), a3(), a4(),
            b1(), b2(1.0f), b3(), b4(),
            c1(), c2(), c3(1.0f), c4(),
            d1(), d2(), d3(), d4(1.0f) {
    }

    template<typename TemplateReal>
    RtMatrix4x4T<TemplateReal>::RtMatrix4x4T(
            TemplateReal _a1, TemplateReal _a2, TemplateReal _a3, TemplateReal _a4,
            TemplateReal _b1, TemplateReal _b2, TemplateReal _b3, TemplateReal _b4,
            TemplateReal _c1, TemplateReal _c2, TemplateReal _c3, TemplateReal _c4,
            TemplateReal _d1, TemplateReal _d2, TemplateReal _d3, TemplateReal _d4) :
            a1(_a1), a2(_a2), a3(_a3), a4(_a4),
            b1(_b1), b2(_b2), b3(_b3), b4(_b4),
            c1(_c1), c2(_c2), c3(_c3), c4(_c4),
            d1(_d1), d2(_d2), d3(_d3), d4(_d4) {
    }

    template<typename TemplateReal>
    template<typename TemplateOther>
    RtMatrix4x4T<TemplateReal>::operator RtMatrix4x4T<TemplateOther>() const {
        return RtMatrix4x4T<TemplateOther>
                (static_cast<TemplateOther>(a1), static_cast<TemplateOther>(a2),
                 static_cast<TemplateOther>(a3), static_cast<TemplateOther>(a4),
                 static_cast<TemplateOther>(b1), static_cast<TemplateOther>(b2),
                 static_cast<TemplateOther>(b3), static_cast<TemplateOther>(b4),
                 static_cast<TemplateOther>(c1), static_cast<TemplateOther>(c2),
                 static_cast<TemplateOther>(c3), static_cast<TemplateOther>(c4),
                 static_cast<TemplateOther>(d1), static_cast<TemplateOther>(d2),
                 static_cast<TemplateOther>(d3), static_cast<TemplateOther>(d4));
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal>::RtMatrix4x4T(const RtMatrix3x3T<TemplateReal> &m) {
        a1 = m.a1;
        a2 = m.a2;
        a3 = m.a3;
        a4 = static_cast<TemplateReal>(0.0);
        b1 = m.b1;
        b2 = m.b2;
        b3 = m.b3;
        b4 = static_cast<TemplateReal>(0.0);
        c1 = m.c1;
        c2 = m.c2;
        c3 = m.c3;
        c4 = static_cast<TemplateReal>(0.0);
        d1 = static_cast<TemplateReal>(0.0);
        d2 = static_cast<TemplateReal>(0.0);
        d3 = static_cast<TemplateReal>(0.0);
        d4 = static_cast<TemplateReal>(1.0);
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal>::RtMatrix4x4T(const RtVector3T<TemplateReal> &scaling,
                                                    const RtQuaternionT<TemplateReal> &rotation,
                                                    const RtVector3T<TemplateReal> &position) {
        RtMatrix4x4T<TemplateReal> m = rotation.GetMatrix();

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

        d1 = static_cast<TemplateReal>(0.0);
        d2 = static_cast<TemplateReal>(0.0);
        d3 = static_cast<TemplateReal>(0.0);
        d4 = static_cast<TemplateReal>(1.0);
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> &RtMatrix4x4T<TemplateReal>::operator*=(const RtMatrix4x4T<TemplateReal> &m) {
        *this = RtMatrix4x4T<TemplateReal>(
                m.a1 * a1 + m.b1 * a2 + m.c1 * a3 + m.d1 * a4,
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

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> RtMatrix4x4T<TemplateReal>::operator*(const TemplateReal &aFloat) const {
        RtMatrix4x4T<TemplateReal> temp(
                a1 * aFloat,
                a2 * aFloat,
                a3 * aFloat,
                a4 * aFloat,
                b1 * aFloat,
                b2 * aFloat,
                b3 * aFloat,
                b4 * aFloat,
                c1 * aFloat,
                c2 * aFloat,
                c3 * aFloat,
                c4 * aFloat,
                d1 * aFloat,
                d2 * aFloat,
                d3 * aFloat,
                d4 * aFloat);
        return temp;
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> RtMatrix4x4T<TemplateReal>::operator+
            (const RtMatrix4x4T<TemplateReal> &m) const {
        RtMatrix4x4T<TemplateReal> temp(
                m.a1 + a1,
                m.a2 + a2,
                m.a3 + a3,
                m.a4 + a4,
                m.b1 + b1,
                m.b2 + b2,
                m.b3 + b3,
                m.b4 + b4,
                m.c1 + c1,
                m.c2 + c2,
                m.c3 + c3,
                m.c4 + c4,
                m.d1 + d1,
                m.d2 + d2,
                m.d3 + d3,
                m.d4 + d4);
        return temp;
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> RtMatrix4x4T<TemplateReal>::operator*(const RtMatrix4x4T<TemplateReal> &m) const {
        RtMatrix4x4T<TemplateReal> temp(*this);
        temp *= m;
        return temp;
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> &RtMatrix4x4T<TemplateReal>::Transpose() {
        std::swap((TemplateReal &) b1, (TemplateReal &) a2);
        std::swap((TemplateReal &) c1, (TemplateReal &) a3);
        std::swap((TemplateReal &) c2, (TemplateReal &) b3);
        std::swap((TemplateReal &) d1, (TemplateReal &) a4);
        std::swap((TemplateReal &) d2, (TemplateReal &) b4);
        std::swap((TemplateReal &) d3, (TemplateReal &) c4);
        return *this;
    }

    template<typename TemplateReal>
    inline TemplateReal RtMatrix4x4T<TemplateReal>::Determinant() const {
        return a1 * b2 * c3 * d4 - a1 * b2 * c4 * d3 + a1 * b3 * c4 * d2 - a1 * b3 * c2 * d4
               + a1 * b4 * c2 * d3 - a1 * b4 * c3 * d2 - a2 * b3 * c4 * d1 + a2 * b3 * c1 * d4
               - a2 * b4 * c1 * d3 + a2 * b4 * c3 * d1 - a2 * b1 * c3 * d4 + a2 * b1 * c4 * d3
               + a3 * b4 * c1 * d2 - a3 * b4 * c2 * d1 + a3 * b1 * c2 * d4 - a3 * b1 * c4 * d2
               + a3 * b2 * c4 * d1 - a3 * b2 * c1 * d4 - a4 * b1 * c2 * d3 + a4 * b1 * c3 * d2
               - a4 * b2 * c3 * d1 + a4 * b2 * c1 * d3 - a4 * b3 * c1 * d2 + a4 * b3 * c2 * d1;
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> &RtMatrix4x4T<TemplateReal>::Inverse() {
        const TemplateReal det = Determinant();
        if (det == static_cast<TemplateReal>(0.0)) {
            const TemplateReal nan = std::numeric_limits<TemplateReal>::quiet_NaN();
            *this = RtMatrix4x4T<TemplateReal>(
                    nan, nan, nan, nan,
                    nan, nan, nan, nan,
                    nan, nan, nan, nan,
                    nan, nan, nan, nan);

            return *this;
        }

        const TemplateReal invdet = static_cast<TemplateReal>(1.0) / det;
        RtMatrix4x4T<TemplateReal> res;
        res.a1 = invdet * (b2 * (c3 * d4 - c4 * d3) + b3 * (c4 * d2 - c2 * d4) + b4 * (c2 * d3 - c3 * d2));
        res.a2 = -invdet * (a2 * (c3 * d4 - c4 * d3) + a3 * (c4 * d2 - c2 * d4) + a4 * (c2 * d3 - c3 * d2));
        res.a3 = invdet * (a2 * (b3 * d4 - b4 * d3) + a3 * (b4 * d2 - b2 * d4) + a4 * (b2 * d3 - b3 * d2));
        res.a4 = -invdet * (a2 * (b3 * c4 - b4 * c3) + a3 * (b4 * c2 - b2 * c4) + a4 * (b2 * c3 - b3 * c2));
        res.b1 = -invdet * (b1 * (c3 * d4 - c4 * d3) + b3 * (c4 * d1 - c1 * d4) + b4 * (c1 * d3 - c3 * d1));
        res.b2 = invdet * (a1 * (c3 * d4 - c4 * d3) + a3 * (c4 * d1 - c1 * d4) + a4 * (c1 * d3 - c3 * d1));
        res.b3 = -invdet * (a1 * (b3 * d4 - b4 * d3) + a3 * (b4 * d1 - b1 * d4) + a4 * (b1 * d3 - b3 * d1));
        res.b4 = invdet * (a1 * (b3 * c4 - b4 * c3) + a3 * (b4 * c1 - b1 * c4) + a4 * (b1 * c3 - b3 * c1));
        res.c1 = invdet * (b1 * (c2 * d4 - c4 * d2) + b2 * (c4 * d1 - c1 * d4) + b4 * (c1 * d2 - c2 * d1));
        res.c2 = -invdet * (a1 * (c2 * d4 - c4 * d2) + a2 * (c4 * d1 - c1 * d4) + a4 * (c1 * d2 - c2 * d1));
        res.c3 = invdet * (a1 * (b2 * d4 - b4 * d2) + a2 * (b4 * d1 - b1 * d4) + a4 * (b1 * d2 - b2 * d1));
        res.c4 = -invdet * (a1 * (b2 * c4 - b4 * c2) + a2 * (b4 * c1 - b1 * c4) + a4 * (b1 * c2 - b2 * c1));
        res.d1 = -invdet * (b1 * (c2 * d3 - c3 * d2) + b2 * (c3 * d1 - c1 * d3) + b3 * (c1 * d2 - c2 * d1));
        res.d2 = invdet * (a1 * (c2 * d3 - c3 * d2) + a2 * (c3 * d1 - c1 * d3) + a3 * (c1 * d2 - c2 * d1));
        res.d3 = -invdet * (a1 * (b2 * d3 - b3 * d2) + a2 * (b3 * d1 - b1 * d3) + a3 * (b1 * d2 - b2 * d1));
        res.d4 = invdet * (a1 * (b2 * c3 - b3 * c2) + a2 * (b3 * c1 - b1 * c3) + a3 * (b1 * c2 - b2 * c1));
        *this = res;

        return *this;
    }

    template<typename TemplateReal>
    inline TemplateReal *RtMatrix4x4T<TemplateReal>::operator[](unsigned int pIndex) {
        if (pIndex > 3) {
            return nullptr;
        }
        switch (pIndex) {
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

    template<typename TemplateReal>
    inline const TemplateReal *RtMatrix4x4T<TemplateReal>::operator[](unsigned int pIndex) const {
        if (pIndex > 3) {
            return nullptr;
        }
        switch (pIndex) {
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

    template<typename TemplateReal>
    inline bool RtMatrix4x4T<TemplateReal>::operator==(const RtMatrix4x4T<TemplateReal> &pMatrix) const {
        return (a1 == pMatrix.a1 && a2 == pMatrix.a2 && a3 == pMatrix.a3 && a4 == pMatrix.a4 &&
                b1 == pMatrix.b1 && b2 == pMatrix.b2 && b3 == pMatrix.b3 && b4 == pMatrix.b4 &&
                c1 == pMatrix.c1 && c2 == pMatrix.c2 && c3 == pMatrix.c3 && c4 == pMatrix.c4 &&
                d1 == pMatrix.d1 && d2 == pMatrix.d2 && d3 == pMatrix.d3 && d4 == pMatrix.d4);
    }

    template<typename TemplateReal>
    inline bool RtMatrix4x4T<TemplateReal>::operator!=(const RtMatrix4x4T<TemplateReal> &pMatrix) const {
        return !(*this == pMatrix);
    }

    template<typename TemplateReal>
    inline bool RtMatrix4x4T<TemplateReal>::Equal(const RtMatrix4x4T<TemplateReal> &m, TemplateReal epsilon) const {
        return
                std::abs(a1 - m.a1) <= epsilon &&
                std::abs(a2 - m.a2) <= epsilon &&
                std::abs(a3 - m.a3) <= epsilon &&
                std::abs(a4 - m.a4) <= epsilon &&
                std::abs(b1 - m.b1) <= epsilon &&
                std::abs(b2 - m.b2) <= epsilon &&
                std::abs(b3 - m.b3) <= epsilon &&
                std::abs(b4 - m.b4) <= epsilon &&
                std::abs(c1 - m.c1) <= epsilon &&
                std::abs(c2 - m.c2) <= epsilon &&
                std::abs(c3 - m.c3) <= epsilon &&
                std::abs(c4 - m.c4) <= epsilon &&
                std::abs(d1 - m.d1) <= epsilon &&
                std::abs(d2 - m.d2) <= epsilon &&
                std::abs(d3 - m.d3) <= epsilon &&
                std::abs(d4 - m.d4) <= epsilon;
    }

#define MATRIX_4_4_DECOMPOSE                                            \
    const RtMatrix4x4T<TemplateReal>& _this = *this;                    \
                                                                        \
                                                                        \
    pPosition.x = _this[0][3];                                          \
    pPosition.y = _this[1][3];                                          \
    pPosition.z = _this[2][3];                                          \
                                                                        \
                                                                        \
    RtVector3T<TemplateReal> vCols[3] = {                               \
        RtVector3T<TemplateReal>(_this[0][0],_this[1][0],_this[2][0]),  \
        RtVector3T<TemplateReal>(_this[0][1],_this[1][1],_this[2][1]),  \
        RtVector3T<TemplateReal>(_this[0][2],_this[1][2],_this[2][2])   \
    };                                                                  \
                                                                        \
    pScaling.x = vCols[0].Length();                                     \
    pScaling.y = vCols[1].Length();                                     \
    pScaling.z = vCols[2].Length();                                     \
                                                                        \
    if (Determinant() < 0) pScaling = -pScaling;                        \
                                                                        \
    if(pScaling.x) vCols[0] /= pScaling.x;                              \
    if(pScaling.y) vCols[1] /= pScaling.y;                              \
    if(pScaling.z) vCols[2] /= pScaling.z;                              \
                                                                        \
    do {} while(false)
    // ^ Ayy it looks do nothing, but it can be avoided from we lost the ';' symbol.
    // Because without this, this macro expanded to an invalid c++ statement. So we have this. ^^
    // Shout out to Assimp Developer Team!

    template<typename TemplateReal>
    inline
    void
    RtMatrix4x4T<TemplateReal>::Decompose(RtVector3T<TemplateReal> &pScaling, RtQuaternionT<TemplateReal> &pRotation,
                                          RtVector3T<TemplateReal> &pPosition) const {
        MATRIX_4_4_DECOMPOSE;

        RtMatrix3x3T<TemplateReal> m(vCols[0].x, vCols[1].x, vCols[2].x,
                                     vCols[0].y, vCols[1].y, vCols[2].y,
                                     vCols[0].z, vCols[1].z, vCols[2].z);

        pRotation = RtQuaternionT<TemplateReal>(m);
    }

    template<typename TemplateReal>
    inline
    void RtMatrix4x4T<TemplateReal>::Decompose(RtVector3T<TemplateReal> &pScaling, RtVector3T<TemplateReal> &pRotation,
                                               RtVector3T<TemplateReal> &pPosition) const {
        MATRIX_4_4_DECOMPOSE;
        const TemplateReal epsilon = eps<TemplateReal>();

        pRotation.y = std::asin(-vCols[0].z);

        TemplateReal C = std::cos(pRotation.y);
        if (std::fabs(C) > epsilon) {
            TemplateReal tan_x = vCols[2].z / C;
            TemplateReal tan_y = vCols[1].z / C;

            pRotation.x = std::atan2(tan_y, tan_x);
            tan_x = vCols[0].x / C;
            tan_y = vCols[0].y / C;
            pRotation.z = std::atan2(tan_y, tan_x);
        } else {
            pRotation.x = 0;

            TemplateReal tan_x = vCols[1].y;
            TemplateReal tan_y = -vCols[1].x;

            pRotation.z = std::atan2(tan_y, tan_x);
        }
    }

#undef MATRIX_4_4_DECOMPOSE

    template<typename TemplateReal>
    inline void
    RtMatrix4x4T<TemplateReal>::Decompose(RtVector3T<TemplateReal> &pScaling, RtVector3T<TemplateReal> &pRotationAxis,
                                          TemplateReal &pRotationAngle, RtVector3T<TemplateReal> &pPosition) const {
        RtQuaternionT<TemplateReal> pRotation;

        Decompose(pScaling, pRotation, pPosition);
        pRotation.Normalize();

        TemplateReal angle_cos = pRotation.w;
        TemplateReal angle_sin = std::sqrt(1.0f - angle_cos * angle_cos);

        pRotationAngle = std::acos(angle_cos) * 2;

        const TemplateReal epsilon = 10e-3f;

        if (std::fabs(angle_sin) < epsilon) angle_sin = 1;

        pRotationAxis.x = pRotation.x / angle_sin;
        pRotationAxis.y = pRotation.y / angle_sin;
        pRotationAxis.z = pRotation.z / angle_sin;
    }

    template<typename TemplateReal>
    inline void RtMatrix4x4T<TemplateReal>::DecomposeNoScaling(RtQuaternionT<TemplateReal> &rotation,
                                                               RtVector3T<TemplateReal> &position) const {
        const RtMatrix4x4T<TemplateReal> &_this = *this;

        position.x = _this[0][3];
        position.y = _this[1][3];
        position.z = _this[2][3];

        rotation = aiQuaterniont<TemplateReal>((RtMatrix3x3T<TemplateReal>) _this);
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> &
    RtMatrix4x4T<TemplateReal>::FromEulerAnglesXYZ(const RtVector3T<TemplateReal> &blubb) {
        return FromEulerAnglesXYZ(blubb.x, blubb.y, blubb.z);
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> &
    RtMatrix4x4T<TemplateReal>::FromEulerAnglesXYZ(TemplateReal x, TemplateReal y, TemplateReal z) {
        RtMatrix4x4T<TemplateReal> &_this = *this;

        TemplateReal cx = std::cos(x);
        TemplateReal sx = std::sin(x);
        TemplateReal cy = std::cos(y);
        TemplateReal sy = std::sin(y);
        TemplateReal cz = std::cos(z);
        TemplateReal sz = std::sin(z);

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

    template<typename TemplateReal>
    inline bool RtMatrix4x4T<TemplateReal>::IsIdentity(const TemplateReal epsilon) const {
        return (a2 <= epsilon && a2 >= -epsilon &&
                a3 <= epsilon && a3 >= -epsilon &&
                a4 <= epsilon && a4 >= -epsilon &&
                b1 <= epsilon && b1 >= -epsilon &&
                b3 <= epsilon && b3 >= -epsilon &&
                b4 <= epsilon && b4 >= -epsilon &&
                c1 <= epsilon && c1 >= -epsilon &&
                c2 <= epsilon && c2 >= -epsilon &&
                c4 <= epsilon && c4 >= -epsilon &&
                d1 <= epsilon && d1 >= -epsilon &&
                d2 <= epsilon && d2 >= -epsilon &&
                d3 <= epsilon && d3 >= -epsilon &&
                a1 <= 1.f + epsilon && a1 >= 1.f - epsilon &&
                b2 <= 1.f + epsilon && b2 >= 1.f - epsilon &&
                c3 <= 1.f + epsilon && c3 >= 1.f - epsilon &&
                d4 <= 1.f + epsilon && d4 >= 1.f - epsilon);
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> &
    RtMatrix4x4T<TemplateReal>::RotationX(TemplateReal a, RtMatrix4x4T<TemplateReal> &out) {
        out = RtMatrix4x4T<TemplateReal>();
        out.b2 = out.c3 = std::cos(a);
        out.b3 = -(out.c2 = std::sin(a));
        return out;
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> &
    RtMatrix4x4T<TemplateReal>::RotationY(TemplateReal a, RtMatrix4x4T<TemplateReal> &out) {
        out = RtMatrix4x4T<TemplateReal>();
        out.a1 = out.c3 = std::cos(a);
        out.c1 = -(out.a3 = std::sin(a));
        return out;
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> &
    RtMatrix4x4T<TemplateReal>::RotationZ(TemplateReal a, RtMatrix4x4T<TemplateReal> &out) {
        out = RtMatrix4x4T<TemplateReal>();
        out.a1 = out.b2 = std::cos(a);
        out.a2 = -(out.b1 = std::sin(a));
        return out;
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> &
    RtMatrix4x4T<TemplateReal>::Rotation(TemplateReal a, const RtVector3T<TemplateReal> &axis,
                                         RtMatrix4x4T<TemplateReal> &out) {
        TemplateReal c = std::cos(a), s = std::sin(a), t = 1 - c;
        TemplateReal x = axis.x, y = axis.y, z = axis.z;

        out.a1 = t * x * x + c;
        out.a2 = t * x * y - s * z;
        out.a3 = t * x * z + s * y;
        out.b1 = t * x * y + s * z;
        out.b2 = t * y * y + c;
        out.b3 = t * y * z - s * x;
        out.c1 = t * x * z - s * y;
        out.c2 = t * y * z + s * x;
        out.c3 = t * z * z + c;
        out.a4 = out.b4 = out.c4 = static_cast<TemplateReal>(0.0);
        out.d1 = out.d2 = out.d3 = static_cast<TemplateReal>(0.0);
        out.d4 = static_cast<TemplateReal>(1.0);

        return out;
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> &
    RtMatrix4x4T<TemplateReal>::Translation(const RtVector3T<TemplateReal> &v, RtMatrix4x4T<TemplateReal> &out) {
        out = RtMatrix4x4T<TemplateReal>();
        out.a4 = v.x;
        out.b4 = v.y;
        out.c4 = v.z;
        return out;
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> &
    RtMatrix4x4T<TemplateReal>::Scaling(const RtVector3T<TemplateReal> &v, RtMatrix4x4T<TemplateReal> &out) {
        out = RtMatrix4x4T<TemplateReal>();
        out.a1 = v.x;
        out.b2 = v.y;
        out.c3 = v.z;
        return out;
    }

    template<typename TemplateReal>
    inline RtMatrix4x4T<TemplateReal> &
    RtMatrix4x4T<TemplateReal>::FromToMatrix(const RtVector3T<TemplateReal> &from, const RtVector3T<TemplateReal> &to,
                                             RtMatrix4x4T<TemplateReal> &out) {
        RtMatrix4x4T<TemplateReal> m3;
//        RtMatrix4x4T<TemplateReal>::FromToMatrix(from, to, m3); // Might cause Infinite Recursion
        out = aiMatrix4x4t<TemplateReal>(m3);
        return out;
    }
}