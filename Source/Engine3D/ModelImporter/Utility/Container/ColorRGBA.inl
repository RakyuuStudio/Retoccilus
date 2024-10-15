#include "ColorRGBA.h"
#include <cmath>

namespace Retoccilus::Engine3D::ModelImporter {
    template <typename Tp>
    inline const RtMIColorRGBA_Template<Tp> &
    RtMIColorRGBA_Template<Tp>::operator+=(const RtMIColorRGBA_Template<Tp> &o) {
        r += o.r;
        g += o.g;
        b += o.b;
        a += o.a;

        return *this;
    }

    template <typename Tp>
    inline const RtMIColorRGBA_Template<Tp> &
    RtMIColorRGBA_Template<Tp>::operator-=(const RtMIColorRGBA_Template<Tp> &o) {
        r -= o.r;
        g -= o.g;
        b -= o.b;
        a -= o.a;
    }

    template <typename Tp>
    inline const RtMIColorRGBA_Template<Tp> &RtMIColorRGBA_Template<Tp>::operator*=(Tp f) {
        r *= f;
        g *= f;
        b *= f;
        a *= f;

        return *this;
    }

    template <typename Tp>
    inline const RtMIColorRGBA_Template<Tp> &RtMIColorRGBA_Template<Tp>::operator/=(Tp f) {
        r /= f;
        g /= f;
        b /= f;
        a /= f;

        return *this;
    }

    template <typename Tp> inline Tp &RtMIColorRGBA_Template<Tp>::operator[](unsigned int i) {
        switch (i) {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
        case 3:
            return a;
        default:
            break;
        }

        return r;
    }

    template <typename Tp> inline Tp RtMIColorRGBA_Template<Tp>::operator[](unsigned int i) const {
        switch (i) {
        case 0:
            return r;
        case 1:
            return g;
        case 2:
            return b;
        case 3:
            return a;
        default:
            break;
        }

        return r;
    }

    template <typename Tp>
    inline bool
    RtMIColorRGBA_Template<Tp>::operator==(const RtMIColorRGBA_Template<Tp> &other) const {
        return (r == other.r) && (g == other.g) && (b == other.b) && (a == other.a);
    }

    template <typename Tp>
    inline bool
    RtMIColorRGBA_Template<Tp>::operator!=(const RtMIColorRGBA_Template<Tp> &other) const {
        return (r != other.r) || (g != other.g) || (b != other.b) || (a != other.a);
    }

    template <typename Tp>
    inline bool
    RtMIColorRGBA_Template<Tp>::operator<(const RtMIColorRGBA_Template<Tp> &other) const {
        return (
            (r < other.r) ||
            (r == other.r &&
             (g < other.g || (g == other.g && (b < other.b || (b == other.b && (a < other.a)))))));
    }

    template <typename Tp>
    inline RtMIColorRGBA_Template<Tp> operator+(const RtMIColorRGBA_Template<Tp> &lhs,
                                                const RtMIColorRGBA_Template<Tp> &rhs) {
        return RtMIColorRGBA_Template<Tp>(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b,
                                          lhs.a + rhs.a);
    }

    template <typename Tp>
    inline RtMIColorRGBA_Template<Tp> operator-(const RtMIColorRGBA_Template<Tp> &lhs,
                                                const RtMIColorRGBA_Template<Tp> &rhs) {
        return RtMIColorRGBA_Template<Tp>(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b,
                                          lhs.a - rhs.a);
    }

    template <typename Tp>
    inline RtMIColorRGBA_Template<Tp> operator*(const RtMIColorRGBA_Template<Tp> &lhs,
                                                const RtMIColorRGBA_Template<Tp> &rhs) {
        return RtMIColorRGBA_Template<Tp>(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b,
                                          lhs.a * rhs.a);
    }

    template <typename Tp>
    inline RtMIColorRGBA_Template<Tp> operator/(const RtMIColorRGBA_Template<Tp> &lhs,
                                                const RtMIColorRGBA_Template<Tp> &rhs) {
        return RtMIColorRGBA_Template<Tp>(lhs.r / rhs.r, lhs.g / rhs.g, lhs.b / rhs.b,
                                          lhs.a / rhs.a);
    }

    template <typename Tp>
    inline RtMIColorRGBA_Template<Tp> operator*(Tp f, const RtMIColorRGBA_Template<Tp> &v) {
        return RtMIColorRGBA_Template<Tp>(f * v.r, f * v.g, f * v.b, f * v.a);
    }

    template <typename Tp>
    inline RtMIColorRGBA_Template<Tp> operator*(const RtMIColorRGBA_Template<Tp> &v, Tp f) {
        return RtMIColorRGBA_Template<Tp>(f * v.r, f * v.g, f * v.b, f * v.a);
    }

    template <typename Tp>
    inline RtMIColorRGBA_Template<Tp> operator/(const RtMIColorRGBA_Template<Tp> &v, Tp f) {
        return v * (1 / f);
    }

    template <typename Tp>
    inline RtMIColorRGBA_Template<Tp> operator/(Tp f, const RtMIColorRGBA_Template<Tp> &v) {
        return RtMIColorRGBA_Template<Tp>(f, f, f, f) / v;
    }

    template <typename Tp>
    inline RtMIColorRGBA_Template<Tp> operator+(const RtMIColorRGBA_Template<Tp> &v, Tp f) {
        return RtMIColorRGBA_Template<Tp>(v.r + f, v.g + f, v.b + f, v.a + f);
    }

    template <typename Tp>
    inline RtMIColorRGBA_Template<Tp> operator-(const RtMIColorRGBA_Template<Tp> &v, Tp f) {
        return RtMIColorRGBA_Template<Tp>(v.r - f, v.g - f, v.b - f, v.a - f);
    }

    template <typename Tp>
    inline RtMIColorRGBA_Template<Tp> operator+(Tp f, const RtMIColorRGBA_Template<Tp> &v) {
        return RtMIColorRGBA_Template<Tp>(f + v.r, f + v.g, f + v.b, f + v.a);
    }

    template <typename Tp>
    inline RtMIColorRGBA_Template<Tp> operator-(Tp f, const RtMIColorRGBA_Template<Tp> &v) {
        return RtMIColorRGBA_Template<Tp>(f - v.r, f - v.g, f - v.b, f - v.a);
    }

    template <typename Tp> inline bool RtMIColorRGBA_Template<Tp>::isBlack() const {
        static const Tp g_eps = 10e-3f;
        return std::fabs(r) < g_eps && std::fabs(g) < g_eps && std::fabs(b) < g_eps;
    }
} // namespace Retoccilus::Engine3D::ModelImporter