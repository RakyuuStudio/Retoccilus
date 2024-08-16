#include <cmath>
#include "Quaternion.h"

namespace Retoccilus::Engine::ThreeDimensionalSupport::Utility {
    template<typename TemplateReal>
    inline RtQuaternionT<TemplateReal> operator*(const RtMatrix4x4T<TemplateReal> &pMatrix,
                                                 const RtQuaternionT<TemplateReal> &pQuaternion) {
        RtQuaternionT<TemplateReal> res;
        res.x = pMatrix.a1 * pQuaternion.x + pMatrix.a2 * pQuaternion.y +
                pMatrix.a3 * pQuaternion.z + pMatrix.a4 * pQuaternion.w;
        res.y = pMatrix.b1 * pQuaternion.x + pMatrix.b2 * pQuaternion.y +
                pMatrix.b3 * pQuaternion.z + pMatrix.b4 * pQuaternion.w;
        res.z = pMatrix.c1 * pQuaternion.x + pMatrix.c2 * pQuaternion.y +
                pMatrix.c3 * pQuaternion.z + pMatrix.c4 * pQuaternion.w;
        res.w = pMatrix.d1 * pQuaternion.x + pMatrix.d2 * pQuaternion.y +
                pMatrix.d3 * pQuaternion.z + pMatrix.d4 * pQuaternion.w;
        return res;
    }

    template<typename TemplateReal>
    bool RtQuaternionT<TemplateReal>::operator==(const RtQuaternionT &o) const {
        return x == o.x && y == o.y && z == o.z && w == o.w;
    }

    template<typename TemplateReal>
    bool RtQuaternionT<TemplateReal>::operator!=(const RtQuaternionT &o) const {
        return *this != o;
    }

    template<typename TemplateReal>
    inline RtQuaternionT<TemplateReal> &
    RtQuaternionT<TemplateReal>::operator*=(const RtMatrix4x4T<TemplateReal> &mat) {
        return (*this = mat * (*this));
    }

    template<typename TemplateReal>
    inline bool RtQuaternionT<TemplateReal>::Equal(const RtQuaternionT &o, TemplateReal epsilon) const {
        return (
                std::abs(x - o.x) <= epsilon &&
                std::abs(y - o.y) <= epsilon &&
                std::abs(z - o.z) <= epsilon &&
                std::abs(w - o.w) <= epsilon
        );
    }

    template<typename TemplateReal>
    inline RtQuaternionT<TemplateReal>::RtQuaternionT(const RtMatrix3x3T<TemplateReal> &pRotMatrix) {
        TemplateReal t = pRotMatrix.a1 + pRotMatrix.b2 + pRotMatrix.c3;

        if (t > static_cast<TemplateReal>(0)) {
            TemplateReal s = std::sqrt(1 + t) * static_cast<TemplateReal>(2.0);
            x = (pRotMatrix.c2 - pRotMatrix.b3) / s;
            y = (pRotMatrix.a3 - pRotMatrix.c1) / s;
            z = (pRotMatrix.b1 - pRotMatrix.a2) / s;
            w = static_cast<TemplateReal>(0.25) * s;
        } else if (pRotMatrix.a1 > pRotMatrix.b2 && pRotMatrix.a1 > pRotMatrix.c3) {
            TemplateReal s = std::sqrt(static_cast<TemplateReal>(1.0) + pRotMatrix.a1 -
                                       pRotMatrix.b2 - pRotMatrix.c3) * static_cast<TemplateReal>(2.0);
            x = static_cast<TemplateReal>(0.25) * s;
            y = (pRotMatrix.b1 + pRotMatrix.a2) / s;
            z = (pRotMatrix.a3 + pRotMatrix.c1) / s;
            w = (pRotMatrix.c2 - pRotMatrix.b3) / s;
        } else if (pRotMatrix.b2 > pRotMatrix.c3) {
            TemplateReal s = std::sqrt(static_cast<TemplateReal>(1.0) + pRotMatrix.b2 - pRotMatrix.a1 - pRotMatrix.c3) *
                             static_cast<TemplateReal>(2.0);
            x = (pRotMatrix.b1 + pRotMatrix.a2) / s;
            y = static_cast<TemplateReal>(0.25) * s;
            z = (pRotMatrix.c2 + pRotMatrix.b3) / s;
            w = (pRotMatrix.a3 - pRotMatrix.c1) / s;
        } else {
            TemplateReal s = std::sqrt(static_cast<TemplateReal>(1.0) + pRotMatrix.c3 - 
                    pRotMatrix.a1 - pRotMatrix.b2) * static_cast<TemplateReal>(2.0);
            x = (pRotMatrix.a3 + pRotMatrix.c1) / s;
            y = (pRotMatrix.c2 + pRotMatrix.b3) / s;
            z = static_cast<TemplateReal>(0.25) * s;
            w = (pRotMatrix.b1 - pRotMatrix.a2) / s;
        }
    }

    template<typename TemplateReal>
    inline RtQuaternionT<TemplateReal>::RtQuaternionT(TemplateReal fPitch, TemplateReal fYaw, TemplateReal fRoll) {
        const TemplateReal fSinPitch(std::sin(fPitch * static_cast<TemplateReal>(0.5)));
        const TemplateReal fCosPitch(std::cos(fPitch * static_cast<TemplateReal>(0.5)));
        const TemplateReal fSinYaw(std::sin(fYaw * static_cast<TemplateReal>(0.5)));
        const TemplateReal fCosYaw(std::cos(fYaw * static_cast<TemplateReal>(0.5)));
        const TemplateReal fSinRoll(std::sin(fRoll * static_cast<TemplateReal>(0.5)));
        const TemplateReal fCosRoll(std::cos(fRoll * static_cast<TemplateReal>(0.5)));
        const TemplateReal fCosPitchCosYaw(fCosPitch * fCosYaw);
        const TemplateReal fSinPitchSinYaw(fSinPitch * fSinYaw);
        x = fSinRoll * fCosPitchCosYaw - fCosRoll * fSinPitchSinYaw;
        y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
        z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
        w = fCosRoll * fCosPitchCosYaw + fSinRoll * fSinPitchSinYaw;
    }

    template<typename TemplateReal>
    inline RtMatrix3x3T <TemplateReal> RtQuaternionT<TemplateReal>::GetMatrix() const {
        RtMatrix3x3T <TemplateReal> resMatrix;
        resMatrix.a1 = static_cast<TemplateReal>(1.0) - static_cast<TemplateReal>(2.0) * (y * y + z * z);
        resMatrix.a2 = static_cast<TemplateReal>(2.0) * (x * y - z * w);
        resMatrix.a3 = static_cast<TemplateReal>(2.0) * (x * z + y * w);
        resMatrix.b1 = static_cast<TemplateReal>(2.0) * (x * y + z * w);
        resMatrix.b2 = static_cast<TemplateReal>(1.0) - static_cast<TemplateReal>(2.0) * (x * x + z * z);
        resMatrix.b3 = static_cast<TemplateReal>(2.0) * (y * z - x * w);
        resMatrix.c1 = static_cast<TemplateReal>(2.0) * (x * z - y * w);
        resMatrix.c2 = static_cast<TemplateReal>(2.0) * (y * z + x * w);
        resMatrix.c3 = static_cast<TemplateReal>(1.0) - static_cast<TemplateReal>(2.0) * (x * x + y * y);
        return resMatrix;
    }

    template<typename TemplateReal>
    inline RtQuaternionT<TemplateReal>::RtQuaternionT(RtVector3T<TemplateReal> axis, TemplateReal angle) {
        axis.Normalize();

        const TemplateReal sin_a = std::sin(angle / 2);
        const TemplateReal cos_a = std::cos(angle / 2);
        x = axis.x * sin_a;
        y = axis.y * sin_a;
        z = axis.z * sin_a;
        w = cos_a;
    }

    template<typename TemplateReal>
    [[maybe_unused]] inline RtQuaternionT<TemplateReal>::RtQuaternionT(RtVector3T<TemplateReal> normalized) {
        x = normalized.x;
        y = normalized.y;
        z = normalized.z;

        const TemplateReal t = static_cast<TemplateReal>(1.0) - (x * x) - (y * y) - (z * z);

        if (t < static_cast<TemplateReal>(0.0)) {
            w = static_cast<TemplateReal>(0.0);
        } else {
            w = std::sqrt(t);
        }
    }

    template<typename TemplateReal>
    inline void RtQuaternionT<TemplateReal>::Interpolate(RtQuaternionT &pOut, const RtQuaternionT &pStart,
                                                         const RtQuaternionT &pEnd, TemplateReal pFactor) {
        TemplateReal cosom = pStart.x * pEnd.x + pStart.y * pEnd.y + pStart.z * pEnd.z + pStart.w * pEnd.w;

        RtQuaternionT end = pEnd;
        if (cosom < static_cast<TemplateReal>(0.0)) {
            cosom = -cosom;
            end.x = -end.x;
            end.y = -end.y;
            end.z = -end.z;
            end.w = -end.w;
        }

        TemplateReal sclp, sclq;

        if ((static_cast<TemplateReal>(1.0) - cosom) > rt_epsilon) {
            TemplateReal omega, sinom;
            omega = std::acos(cosom);
            sinom = std::sin(omega);
            sclp = std::sin((static_cast<TemplateReal>(1.0) - pFactor) * omega) / sinom;
            sclq = std::sin(pFactor * omega) / sinom;
        } else {
            sclp = static_cast<TemplateReal>(1.0) - pFactor;
            sclq = pFactor;
        }

        pOut.x = sclp * pStart.x + sclq * end.x;
        pOut.y = sclp * pStart.y + sclq * end.y;
        pOut.z = sclp * pStart.z + sclq * end.z;
        pOut.w = sclp * pStart.w + sclq * end.w;
    }

    template<typename TemplateReal>
    inline RtQuaternionT<TemplateReal> &RtQuaternionT<TemplateReal>::Normalize() {
        const TemplateReal mag = std::sqrt(x * x + y * y + z * z + w * w);
        if (mag) {
            const TemplateReal invMag = static_cast<TemplateReal>(1.0) / mag;
            x *= invMag;
            y *= invMag;
            z *= invMag;
            w *= invMag;
        }
        return *this;
    }

    template<typename TemplateReal>
    inline RtQuaternionT<TemplateReal> RtQuaternionT<TemplateReal>::operator*(const RtQuaternionT &t) const {
        return RtQuaternionT(w * t.w - x * t.x - y * t.y - z * t.z,
                             w * t.x + x * t.w + y * t.z - z * t.y,
                             w * t.y + y * t.w + z * t.x - x * t.z,
                             w * t.z + z * t.w + x * t.y - y * t.x);
    }

    template<typename TemplateReal>
    inline RtQuaternionT<TemplateReal> &RtQuaternionT<TemplateReal>::Conjugate() {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    template<typename TemplateReal>
    inline RtVector3T<TemplateReal> RtQuaternionT<TemplateReal>::Rotate(const RtVector3T <TemplateReal> &v) const {
        RtQuaternionT q2(0.f, v.x, v.y, v.z), q = *this, qinv = q;
        qinv.Conjugate();

        q = q * q2 * qinv;
        return aiVector3t<TemplateReal>(q.x, q.y, q.z);
    }
}