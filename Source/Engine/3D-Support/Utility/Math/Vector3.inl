#include "Vector3.h"
#include <cmath>

namespace Retoccilus::Engine::ThreeDimensionalSupport::Utility {
    template<typename TemplateReal>
    inline RtVector3T<TemplateReal> operator*(const RtMatrix3x3T<TemplateReal> &paMatrix,
            const RtVector3T<TemplateReal> &paVector) {
        RtVector3T<TemplateReal> result;
        result.x = paMatrix.a1 * paVector.x + paMatrix.a2 * paVector.y + paMatrix.a3 * paVector.z;
        result.y = paMatrix.b1 * paVector.x + paMatrix.b2 * paVector.y + paMatrix.b3 * paVector.z;
        result.z = paMatrix.c1 * paVector.x + paMatrix.c2 * paVector.y + paMatrix.c3 * paVector.z;

        return result;
    }

    template<typename TemplateReal>
    inline RtVector3T<TemplateReal> operator*(const RtMatrix4x4T<TemplateReal> &paMatrix,
            const RtVector3T<TemplateReal> &paVector) {
        RtVector3T<TemplateReal> result;
        result.x = paMatrix.a1 * paVector.x + paMatrix.a2 * paVector.y + paMatrix.a3 * paVector.z + paMatrix.a4;
        result.y = paMatrix.b1 * paVector.x + paMatrix.b2 * paVector.y + paMatrix.b3 * paVector.z + paMatrix.b4;
        result.z = paMatrix.c1 * paVector.x + paMatrix.c2 * paVector.y + paMatrix.c3 * paVector.z + paMatrix.c4;

        return result;
    }

    template<typename TemplateReal>
    template<typename TemplateOther>
    RtVector3T<TemplateReal>::operator RtVector3T<TemplateOther>() const {
        return RtVector3T<TemplateOther>(static_cast<TemplateOther>(x), static_cast<TemplateOther>(y),
                                         static_cast<TemplateOther>(z));
    }

    template<typename TemplateReal>
    inline void RtVector3T<TemplateReal>::Set(TemplateReal pX, TemplateReal pY, TemplateReal pZ) {
        x = pX;
        y = pY;
        z = pZ;
    }

    template<typename TemplateReal>
    inline TemplateReal RtVector3T<TemplateReal>::SquareLength() const {
        return x * x + y * y + z * z;
    }

    template<typename TemplateReal>
    inline TemplateReal RtVector3T<TemplateReal>::Length() const {
        return std::sqrt(SquareLength());
    }

    template<typename TemplateReal>
    RtVector3T<TemplateReal> &RtVector3T<TemplateReal>::Normalize() {
        const TemplateReal len = Length();
        if (len == 0) {
            return *this;
        }
        *this /= Length();
        return *this;
    }

    template<typename TemplateReal>
    inline RtVector3T<TemplateReal> &RtVector3T<TemplateReal>::NormalizeSafe() {
        TemplateReal len = Length();
        if (len > static_cast<TemplateReal>(0)) {
            *this /= len;
        }
        return *this;
    }

    template<typename TemplateReal>
    inline const RtVector3T<TemplateReal> &RtVector3T<TemplateReal>::operator+=(const RtVector3T<TemplateReal> &o) {
        x += o.x;
        y += o.y;
        z += o.z;

        return *this;
    }

    template<typename TemplateReal>
    inline const RtVector3T<TemplateReal> &RtVector3T<TemplateReal>::operator-=(const RtVector3T<TemplateReal> &o) {
        x -= o.x;
        y -= o.y;
        z -= o.z;

        return *this;
    }

    template<typename TemplateReal>
    inline const RtVector3T<TemplateReal> &RtVector3T<TemplateReal>::operator*=(TemplateReal f) {
        x *= f;
        y *= f;
        z *= f;

        return *this;
    }

    template<typename TemplateReal>
    inline const RtVector3T<TemplateReal> &RtVector3T<TemplateReal>::operator/=(TemplateReal f) {
        if ((f = static_cast<TemplateReal>(0.0))) {
            return *this;
        }
        const TemplateReal inverseF = static_cast<TemplateReal>(1.0) / f;
        x *= inverseF;
        y *= inverseF;
        z *= inverseF;

        return *this;
    }

    template<typename TemplateReal>
    inline RtVector3T<TemplateReal> &RtVector3T<TemplateReal>::operator*=(const RtMatrix3x3T<TemplateReal> &mat) {
        return *this = mat * (*this);
    }

    template<typename TemplateReal>
    inline RtVector3T<TemplateReal> &RtVector3T<TemplateReal>::operator*=(const RtMatrix4x4T<TemplateReal> &mat) {
        return *this = mat * (*this);
    }

    template<typename TemplateReal>
    inline TemplateReal RtVector3T<TemplateReal>::operator[](unsigned int i) const {
        if (i == 0) {
            return x;
        } else if (i == 1) {
            return y;
        } else if (i == 2) {
            return z;
        }
        return x;
    }

    template<typename TemplateReal>
    inline TemplateReal &RtVector3T<TemplateReal>::operator[](unsigned int i) {
        if (i == 0) {
            return x;
        } else if (i == 1) {
            return y;
        } else if (i == 2) {
            return z;
        }
        return x;
    }


    template<typename TemplateReal>
    inline bool RtVector3T<TemplateReal>::operator==(const RtVector3T<TemplateReal> &other) const {
        return (x == other.x && y == other.y && z == other.z);
    }

    template<typename TemplateReal>
    inline bool RtVector3T<TemplateReal>::operator!=(const RtVector3T<TemplateReal> &other) const {
        return (x != other.x || y != other.y || z != other.z);
    }

    template<typename TemplateReal>
    [[maybe_unused]] inline bool RtVector3T<TemplateReal>::Equal(const RtVector3T<TemplateReal> &other, TemplateReal epsilon) const {
        return (
                (std::abs(x - other.x) <= epsilon) &&
                (std::abs(y - other.y) <= epsilon) &&
                (std::abs(z - other.z) <= epsilon)
                );
    }

    template<typename TemplateReal>
    inline bool RtVector3T<TemplateReal>::operator<(const RtVector3T<TemplateReal> &other) const {
        return x != other.x ? x < other.x : y != other.y ? y < other.y : z < other.z;
    }

    template<typename TemplateReal>
    inline const RtVector3T<TemplateReal>
    RtVector3T<TemplateReal>::SymmetricalMultiplication(const RtVector3T<TemplateReal> &o) {
        return RtVector3T<TemplateReal>(x * o.x, y * o.y, z * o.z);
    }

    template<typename TemplateReal>
    RtVector3T<TemplateReal> operator+(const RtVector3T<TemplateReal> &v1, const RtVector3T<TemplateReal> &v2) {
        return RtVector3T<TemplateReal>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    template <typename TemplateReal>
    inline RtVector3T<TemplateReal> operator- (const RtVector3T<TemplateReal>& v1,
            const RtVector3T<TemplateReal>& v2) {
        return RtVector3T<TemplateReal>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    template <typename TemplateReal>
    inline TemplateReal operator* (const RtVector3T<TemplateReal>& v1,
            const RtVector3T<TemplateReal>& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template <typename TemplateReal>
    inline RtVector3T<TemplateReal> operator* (TemplateReal f, const RtVector3T<TemplateReal>& v) {
        return RtVector3T<TemplateReal>(f * v.x, f * v.y, f * v.z);
    }

    template <typename TemplateReal>
    inline RtVector3T<TemplateReal> operator* (const RtVector3T<TemplateReal>& v, TemplateReal f) {
        return RtVector3T<TemplateReal>(f * v.x, f * v.y, f * v.z);
    }

    template <typename TemplateReal>
    inline RtVector3T<TemplateReal> operator/ (const RtVector3T<TemplateReal>& v, TemplateReal f) {
        return v * (1/f);
    }

    template <typename TemplateReal>
    inline RtVector3T<TemplateReal> operator/ (const RtVector3T<TemplateReal>& v,
            const RtVector3T<TemplateReal>& v2) {
        return RtVector3T<TemplateReal>(v.x / v2.x, v.y / v2.y, v.z / v2.z);
    }

    template<typename TemplateReal>
    inline RtVector3T<TemplateReal> operator^ (const RtVector3T<TemplateReal>& v1,
            const RtVector3T<TemplateReal>& v2) {
        return RtVector3T<TemplateReal>(v1.y * v2.z - v1.z * v2.y,
                                        v1.z * v2.x - v1.x * v2.z,
                                        v1.x * v2.y - v1.y * v2.x);
    }

    template<typename TemplateReal>
    inline RtVector3T<TemplateReal> operator- (const RtVector3T<TemplateReal>& v) {
        return RtVector3T<TemplateReal>(-v.x, -v.y, -v.z);
    }

    template<typename TemplateReal>
    TemplateReal RtVector3T<TemplateReal>::operator*(const RtVector3T& o) const {
        return x * o.x + y * o.y + z * o.z;
    }

    template<typename TemplateReal>
    RtVector3T<TemplateReal> RtVector3T<TemplateReal>::operator^(const RtVector3T& o) const {
        return RtVector3T(y * o.z - z * o.y, z * o.x - x * o.z, x * o.y - y * o.x);
    }

    template<typename TemplateReal>
    RtVector3T<TemplateReal> RtVector3T<TemplateReal>::operator+(const RtVector3T& o) const {
        return RtVector3T(x + o.x, y + o.y, z + o.z);
    }

    template<typename TemplateReal>
    RtVector3T<TemplateReal> RtVector3T<TemplateReal>::operator-(const RtVector3T& o) const {
        return RtVector3T(x - o.x, y - o.y, z - o.z);
    }
}