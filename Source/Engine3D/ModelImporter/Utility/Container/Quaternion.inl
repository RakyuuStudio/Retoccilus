#include "Quaternion.h"
#include <cmath>

namespace Retoccilus::Engine3D::ModelImporter {
    template <typename Tp>
    RtMIQuaternion_Template<Tp>::RtMIQuaternion_Template(Tp pw, Tp px, Tp py, Tp pz) : w(pw), x(px), y(py), z(pz) {}

    template <typename Tp> RtMIQuaternion_Template<Tp>::RtMIQuaternion_Template() noexcept : w(1.0), x(), y(), z() {}

    template <typename Tp>
    inline RtMIQuaternion_Template<Tp> operator*(const RtMIMatrix4x4_Template<Tp> &pMatrix,
                                                 const RtMIQuaternion_Template<Tp> &pQuaternion) {
        RtMIQuaternion_Template<Tp> res;
        res.x = pMatrix.a1 * pQuaternion.x + pMatrix.a2 * pQuaternion.y + pMatrix.a3 * pQuaternion.z +
                pMatrix.a4 * pQuaternion.w;
        res.y = pMatrix.b1 * pQuaternion.x + pMatrix.b2 * pQuaternion.y + pMatrix.b3 * pQuaternion.z +
                pMatrix.b4 * pQuaternion.w;
        res.z = pMatrix.c1 * pQuaternion.x + pMatrix.c2 * pQuaternion.y + pMatrix.c3 * pQuaternion.z +
                pMatrix.c4 * pQuaternion.w;
        res.w = pMatrix.d1 * pQuaternion.x + pMatrix.d2 * pQuaternion.y + pMatrix.d3 * pQuaternion.z +
                pMatrix.d4 * pQuaternion.w;
        return res;
    }

    template <typename Tp> bool RtMIQuaternion_Template<Tp>::operator==(const RtMIQuaternion_Template &o) const {
        return x == o.x && y == o.y && z == o.z && w == o.w;
    }

    template <typename Tp> bool RtMIQuaternion_Template<Tp>::operator!=(const RtMIQuaternion_Template &o) const {
        return *this != o;
    }

    template <typename Tp>
    inline RtMIQuaternion_Template<Tp> &RtMIQuaternion_Template<Tp>::operator*=(const RtMIMatrix4x4_Template<Tp> &mat) {
        return (*this = mat * (*this));
    }

    template <typename Tp>
    inline bool RtMIQuaternion_Template<Tp>::Equal(const RtMIQuaternion_Template &o, Tp epsilon) const {
        return std::abs(x - o.x) <= epsilon && std::abs(y - o.y) <= epsilon && std::abs(z - o.z) <= epsilon &&
               std::abs(w - o.w) <= epsilon;
    }

    template <typename Tp>
    inline RtMIQuaternion_Template<Tp>::RtMIQuaternion_Template(const RtMIMatrix3x3_Template<Tp> &pRotMatrix) {
        Tp t = pRotMatrix.a1 + pRotMatrix.b2 + pRotMatrix.c3;

        if (t > static_cast<Tp>(0)) {
            Tp s = std::sqrt(1 + t) * static_cast<Tp>(2.0);
            x    = (pRotMatrix.c2 - pRotMatrix.b3) / s;
            y    = (pRotMatrix.a3 - pRotMatrix.c1) / s;
            z    = (pRotMatrix.b1 - pRotMatrix.a2) / s;
            w    = static_cast<Tp>(0.25) * s;
        }
        else if (pRotMatrix.a1 > pRotMatrix.b2 && pRotMatrix.a1 > pRotMatrix.c3) {
            // Column 0:
            Tp s =
                std::sqrt(static_cast<Tp>(1.0) + pRotMatrix.a1 - pRotMatrix.b2 - pRotMatrix.c3) * static_cast<Tp>(2.0);
            x = static_cast<Tp>(0.25) * s;
            y = (pRotMatrix.b1 + pRotMatrix.a2) / s;
            z = (pRotMatrix.a3 + pRotMatrix.c1) / s;
            w = (pRotMatrix.c2 - pRotMatrix.b3) / s;
        } else if (pRotMatrix.b2 > pRotMatrix.c3) {
            Tp s =
                std::sqrt(static_cast<Tp>(1.0) + pRotMatrix.b2 - pRotMatrix.a1 - pRotMatrix.c3) * static_cast<Tp>(2.0);
            x = (pRotMatrix.b1 + pRotMatrix.a2) / s;
            y = static_cast<Tp>(0.25) * s;
            z = (pRotMatrix.c2 + pRotMatrix.b3) / s;
            w = (pRotMatrix.a3 - pRotMatrix.c1) / s;
        } else {
            Tp s =
                std::sqrt(static_cast<Tp>(1.0) + pRotMatrix.c3 - pRotMatrix.a1 - pRotMatrix.b2) * static_cast<Tp>(2.0);
            x = (pRotMatrix.a3 + pRotMatrix.c1) / s;
            y = (pRotMatrix.c2 + pRotMatrix.b3) / s;
            z = static_cast<Tp>(0.25) * s;
            w = (pRotMatrix.b1 - pRotMatrix.a2) / s;
        }
    }

    template <typename Tp> inline RtMIQuaternion_Template<Tp>::RtMIQuaternion_Template(Tp fPitch, Tp fYaw, Tp fRoll) {
        const Tp fSinPitch(std::sin(fPitch * static_cast<Tp>(0.5)));
        const Tp fCosPitch(std::cos(fPitch * static_cast<Tp>(0.5)));
        const Tp fSinYaw(std::sin(fYaw * static_cast<Tp>(0.5)));
        const Tp fCosYaw(std::cos(fYaw * static_cast<Tp>(0.5)));
        const Tp fSinRoll(std::sin(fRoll * static_cast<Tp>(0.5)));
        const Tp fCosRoll(std::cos(fRoll * static_cast<Tp>(0.5)));
        const Tp fCosPitchCosYaw(fCosPitch * fCosYaw);
        const Tp fSinPitchSinYaw(fSinPitch * fSinYaw);
        x = fSinRoll * fCosPitchCosYaw - fCosRoll * fSinPitchSinYaw;
        y = fCosRoll * fSinPitch * fCosYaw + fSinRoll * fCosPitch * fSinYaw;
        z = fCosRoll * fCosPitch * fSinYaw - fSinRoll * fSinPitch * fCosYaw;
        w = fCosRoll * fCosPitchCosYaw + fSinRoll * fSinPitchSinYaw;
    }

    template <typename Tp> inline RtMIMatrix3x3_Template<Tp> RtMIQuaternion_Template<Tp>::GetMatrix() const {
        RtMIMatrix3x3_Template<Tp> resMatrix;
        resMatrix.a1 = static_cast<Tp>(1.0) - static_cast<Tp>(2.0) * (y * y + z * z);
        resMatrix.a2 = static_cast<Tp>(2.0) * (x * y - z * w);
        resMatrix.a3 = static_cast<Tp>(2.0) * (x * z + y * w);
        resMatrix.b1 = static_cast<Tp>(2.0) * (x * y + z * w);
        resMatrix.b2 = static_cast<Tp>(1.0) - static_cast<Tp>(2.0) * (x * x + z * z);
        resMatrix.b3 = static_cast<Tp>(2.0) * (y * z - x * w);
        resMatrix.c1 = static_cast<Tp>(2.0) * (x * z - y * w);
        resMatrix.c2 = static_cast<Tp>(2.0) * (y * z + x * w);
        resMatrix.c3 = static_cast<Tp>(1.0) - static_cast<Tp>(2.0) * (x * x + y * y);

        return resMatrix;
    }

    template <typename Tp>
    inline RtMIQuaternion_Template<Tp>::RtMIQuaternion_Template(RtMIVector3D_Template<Tp> axis, Tp angle) {
        axis.Normalize();

        const Tp sin_a = std::sin(angle / 2);
        const Tp cos_a = std::cos(angle / 2);
        x              = axis.x * sin_a;
        y              = axis.y * sin_a;
        z              = axis.z * sin_a;
        w              = cos_a;
    }

    template <typename Tp>
    inline RtMIQuaternion_Template<Tp>::RtMIQuaternion_Template(RtMIVector3D_Template<Tp> normalized) {
        x = normalized.x;
        y = normalized.y;
        z = normalized.z;

        const Tp t = static_cast<Tp>(1.0) - (x * x) - (y * y) - (z * z);

        if (t < static_cast<Tp>(0.0)) {
            w = static_cast<Tp>(0.0);
        } else
            w = std::sqrt(t);
    }

    template <typename Tp>
    inline void RtMIQuaternion_Template<Tp>::Interpolate(RtMIQuaternion_Template &pOut,
                                                         const RtMIQuaternion_Template &pStart,
                                                         const RtMIQuaternion_Template &pEnd, Tp pFactor) {
        Tp cosom = pStart.x * pEnd.x + pStart.y * pEnd.y + pStart.z * pEnd.z + pStart.w * pEnd.w;

        RtMIQuaternion_Template end = pEnd;
        if (cosom < static_cast<Tp>(0.0)) {
            cosom = -cosom;
            end.x = -end.x;
            end.y = -end.y;
            end.z = -end.z;
            end.w = -end.w;
        }

        Tp sclp, sclq;

        if ((static_cast<Tp>(1.0) - cosom) > g_epsilon)
        {
            Tp omega, sinom;
            omega = std::acos(cosom);
            sinom = std::sin(omega);
            sclp  = std::sin((static_cast<Tp>(1.0) - pFactor) * omega) / sinom;
            sclq  = std::sin(pFactor * omega) / sinom;
        } else {
            sclp = static_cast<Tp>(1.0) - pFactor;
            sclq = pFactor;
        }

        pOut.x = sclp * pStart.x + sclq * end.x;
        pOut.y = sclp * pStart.y + sclq * end.y;
        pOut.z = sclp * pStart.z + sclq * end.z;
        pOut.w = sclp * pStart.w + sclq * end.w;
    }

    template <typename Tp> inline RtMIQuaternion_Template<Tp> &RtMIQuaternion_Template<Tp>::Normalize() {
        const Tp mag = std::sqrt(x * x + y * y + z * z + w * w);
        if (mag) {
            const Tp invMag = static_cast<Tp>(1.0) / mag;
            x *= invMag;
            y *= invMag;
            z *= invMag;
            w *= invMag;
        }
        return *this;
    }

    template <typename Tp>
    inline RtMIQuaternion_Template<Tp> RtMIQuaternion_Template<Tp>::operator*(const RtMIQuaternion_Template &t) const {
        return RtMIQuaternion_Template(w * t.w - x * t.x - y * t.y - z * t.z, w * t.x + x * t.w + y * t.z - z * t.y,
                                       w * t.y + y * t.w + z * t.x - x * t.z, w * t.z + z * t.w + x * t.y - y * t.x);
    }

    template <typename Tp> inline RtMIQuaternion_Template<Tp> &RtMIQuaternion_Template<Tp>::Conjugate() {
        x = -x;
        y = -y;
        z = -z;
        return *this;
    }

    template <typename Tp>
    inline RtMIVector3D_Template<Tp> RtMIQuaternion_Template<Tp>::Rotate(const RtMIVector3D_Template<Tp> &v) const {
        RtMIQuaternion_Template q2(0.f, v.x, v.y, v.z), q = *this, qinv = q;
        qinv.Conjugate();

        q = q * q2 * qinv;
        return RtMIVector3D_Template<Tp>(q.x, q.y, q.z);
    }
} // namespace Retoccilus::Engine3D::ModelImporter