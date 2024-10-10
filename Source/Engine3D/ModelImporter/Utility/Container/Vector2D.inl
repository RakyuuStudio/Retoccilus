#include "Vector2D.h"

namespace Retoccilus::Engine3D::ModelImporter {
    template<typename Tp>
    RtMIVector2D_Template<Tp>::RtMIVector2D_Template() : x(0), y(0) {}

    template<typename Tp>
    RtMIVector2D_Template<Tp>::RtMIVector2D_Template(Tp _x, Tp _y) : x(_x), y(_y) {}

    template<typename Tp>
    RtMIVector2D_Template<Tp>::RtMIVector2D_Template(Tp _xyz) : x(_xyz), y(_xyz) {}

    template<typename Tp>
    inline void RtMIVector2D_Template<Tp>::setData(Tp X, Tp Y) {
        x = X;
        y = Y;
    }

    template<typename Tp>
    template<typename TOther>
    RtMIVector2D_Template<Tp>::operator RtMIVector2D_Template<TOther>() const {
        return RtMIVector2D_Template<TOther>(
                static_cast<TOther>(x),
                static_cast<TOther>(y)
        );
    }

    template<typename Tp>
    inline Tp RtMIVector2D_Template<Tp>::squareLength() const {
        return x * x + y * y;
    }

    template<typename Tp>
    inline Tp RtMIVector2D_Template<Tp>::length() {
        return std::sqrt(this->squareLength());
    }

    template<typename Tp>
    inline RtMIVector2D_Template<Tp> &RtMIVector2D_Template<Tp>::normalize() {
        *this /= this->length();
        return *this;
    }

    template<typename Tp>
    inline const RtMIVector2D_Template<Tp> &RtMIVector2D_Template<Tp>::operator+=(const RtMIVector2D_Template<Tp> &o) {
        x += o.x;
        y += o.y;
        return *this;
    }

    template<typename Tp>
    inline const RtMIVector2D_Template<Tp> &RtMIVector2D_Template<Tp>::operator-=(const RtMIVector2D_Template<Tp> &o) {
        x -= o.x;
        y -= o.y;
        return *this;
    }

    template<typename Tp>
    inline const RtMIVector2D_Template<Tp> &RtMIVector2D_Template<Tp>::operator*=(Tp f) {
        x *= f;
        y *= f;
        return *this;
    }

    template<typename Tp>
    inline const RtMIVector2D_Template<Tp> &RtMIVector2D_Template<Tp>::operator/=(Tp f) {
        x /= f;
        y /= f;
        return *this;
    }

    template<typename Tp>
    inline Tp RtMIVector2D_Template<Tp>::operator[](unsigned int i) const {
        if (i == 0) {
            return x;
        } else if (i == 1) {
            return y;
        }
        return x;
    }

    template<typename Tp>
    inline bool RtMIVector2D_Template<Tp>::operator==(const RtMIVector2D_Template<Tp> &other) const {
        return x == other.x && y == other.y;
    }

    template<typename Tp>
    inline bool RtMIVector2D_Template<Tp>::operator!=(const RtMIVector2D_Template<Tp> &other) const {
        return x != other.x || y != other.y;
    }

    template<typename Tp>
    inline bool RtMIVector2D_Template<Tp>::equal(const RtMIVector2D_Template<Tp> &other, Tp epsilon) const {
        return (
                std::abs(x - other.x) <= epsilon &&
                std::abs(y - other.y) <= epsilon
        );
    }

    template<typename Tp>
    inline RtMIVector2D_Template<Tp> &RtMIVector2D_Template<Tp>::operator=(Tp f) {
        x = f;
        y = f;
        return *this;
    }

    template<typename Tp>
    inline const RtMIVector2D_Template<Tp> RtMIVector2D_Template<Tp>::symmetricMultiply(const RtMIVector2D_Template<Tp> &o) {
        return RtMIVector2D_Template(x * o.x, y * o.y);
    }

    template<typename Tp>
    inline RtMIVector2D_Template<Tp> operator+(const RtMIVector2D_Template<Tp> &v1,
                                               const RtMIVector2D_Template<Tp> &v2) {
        return RtMIVector2D_Template<Tp>(v1.x + v2.x, v1.y + v2.y);
    }

    template<typename Tp>
    inline RtMIVector2D_Template<Tp> operator-(const RtMIVector2D_Template<Tp> &v1,
                                               const RtMIVector2D_Template<Tp> &v2) {
        return RtMIVector2D_Template<Tp>(v1.x - v2.x, v1.y - v2.y);
    }

    template<typename Tp>
    inline Tp operator*(const RtMIVector2D_Template<Tp> &v1, const RtMIVector2D_Template<Tp> &v2) {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template<typename Tp>
    inline RtMIVector2D_Template<Tp> operator*(Tp f, const RtMIVector2D_Template<Tp> &v) {
        return RtMIVector2D_Template<Tp>(f * v.x, f * v.y);
    }

    template<typename Tp>
    inline RtMIVector2D_Template<Tp> operator*(const RtMIVector2D_Template<Tp> &v, Tp f) {
        return RtMIVector2D_Template<Tp>(f * v.x, f * v.y);
    }

    template<typename Tp>
    inline RtMIVector2D_Template<Tp> operator/(const RtMIVector2D_Template<Tp> &v, Tp f) {
        return v * (1 / f);
    }

    template<typename Tp>
    inline RtMIVector2D_Template<Tp> operator/(const RtMIVector2D_Template<Tp> &v, const RtMIVector2D_Template<Tp> &v2) {
        return RtMIVector2D_Template<Tp>(v.x / v2.x, v.y / v2.y);
    }

    template<typename Tp>
    inline RtMIVector2D_Template<Tp> operator-(const RtMIVector2D_Template<Tp> &v) {
        return RtMIVector2D_Template<Tp>(-v.x, -v.y);
    }
}
