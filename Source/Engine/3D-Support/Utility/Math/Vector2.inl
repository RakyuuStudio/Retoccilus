#include "Vector2.h"
#include <cmath>
#include <stdexcept>

namespace Retoccilus::Engine::ThreeDimensionalSupport::Utility {
    template<typename TemplateReal>
    template<typename TpOther>
    RtVector2T<TemplateReal>::operator RtVector2T<TpOther>() const {
        return RtVector2T<TpOther>(static_cast<TpOther>(x), static_cast<TpOther>(y));
    }

    template<typename TemplateReal>
    inline void RtVector2T<TemplateReal>::Set(TemplateReal pX, TemplateReal pY) {
        x = pX;
        y = pY;
    }

    template<typename TemplateReal>
    inline TemplateReal RtVector2T<TemplateReal>::SquareLength() const {
        return x * x + y * y;
    }

    template<typename TemplateReal>
    inline TemplateReal RtVector2T<TemplateReal>::Length() const {
        return std::sqrt(SquareLength());
    }

    template<typename TemplateReal>
    inline RtVector2T<TemplateReal> &RtVector2T<TemplateReal>::Normalize() {
        *this /= Length();
        return *this;
    }

    template<typename TemplateReal>
    inline const RtVector2T<TemplateReal> &
    RtVector2T<TemplateReal>::operator+=(const RtVector2T<TemplateReal> &o) {
        x += o.x;
        y += o.y;
        return *this;
    }

    template<typename TemplateReal>
    inline const RtVector2T<TemplateReal> &RtVector2T<TemplateReal>::operator-=(const RtVector2T<TemplateReal> &o) {
        x -= o.x;
        y -= o.y;
        return *this;
    }

    template<typename TemplateReal>
    inline const RtVector2T<TemplateReal> & RtVector2T<TemplateReal>::operator*=(TemplateReal f) {
        x *= f;
        y *= f;
        return *this;
    }

    template<typename TemplateReal>
    inline const RtVector2T<TemplateReal> &RtVector2T<TemplateReal>::operator/=(TemplateReal f) {
        x /= f;
        y /= f;
        return *this;
    }

    template<typename TemplateReal>
    inline TemplateReal RtVector2T<TemplateReal>::operator[](unsigned int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        throw std::out_of_range("Index out of range for RtVector2T.");
    }

    template<typename TemplateReal>
    inline bool RtVector2T<TemplateReal>::operator!=(const RtVector2T<TemplateReal> &other) const {
        return (x != other.x || y != other.y);
    }

    template<typename TemplateReal>
    inline bool RtVector2T<TemplateReal>::operator==(const RtVector2T<TemplateReal> &other) const {
        return (x == other.x && y == other.y);
    }

    template<typename TemplateReal>
    inline bool RtVector2T<TemplateReal>::Equal(const RtVector2T<TemplateReal> &other, TemplateReal epsilon) const {
        return (
            std::abs(x - other.x) <= epsilon && std::abs(y - other.y) <= epsilon
        );
    }

    template<typename TemplateReal>
    inline RtVector2T<TemplateReal> &RtVector2T<TemplateReal>::operator=(TemplateReal f) {
        x = f;
        y = f;
        return *this;
    }

    template<typename TemplateReal>
    inline const RtVector2T<TemplateReal>
            RtVector2T<TemplateReal>::SymmetricalMultiply(const RtVector2T &o) {
        return RtVector2T(x * o.x, y * o.y);
    }

    template<typename TemplateReal>
    inline RtVector2T<TemplateReal> operator+(const RtVector2T<TemplateReal> &vx, const RtVector2T<TemplateReal> &vy) {
        return RtVector2T<TemplateReal>(vx.x + vy.x, vx.y + vy.y);
    }

    template<typename TemplateReal>
    inline RtVector2T<TemplateReal> operator-(const RtVector2T<TemplateReal> &vx, const RtVector2T<TemplateReal> &vy) {
        return RtVector2T<TemplateReal>(vx.x - vy.x, vx.y - vy.y);
    }

    template <typename TemplateReal>
    inline RtVector2T<TemplateReal> operator-( const RtVector2T<TemplateReal>& v) {
        return RtVector2T<TemplateReal>(-v.x, -v.y);
    }
    
    template<typename TemplateReal>
    inline RtVector2T<TemplateReal> operator*(const RtVector2T<TemplateReal> &vx, const RtVector2T<TemplateReal> &vy) {
        return RtVector2T<TemplateReal>(vx.x * vy.x, vx.y * vy.y);
    }

    template<typename TemplateReal>
    inline RtVector2T<TemplateReal> operator*(TemplateReal f, const RtVector2T<TemplateReal> &v) {
        return RtVector2T<TemplateReal>(f * v.x, f * v.y);
    }

    template<typename TemplateReal>
    inline RtVector2T<TemplateReal> operator*(const RtVector2T<TemplateReal> &v, TemplateReal f) {
        return RtVector2T<TemplateReal>(f * v.x, f * v.y);
    }

    template <typename TemplateReal>
    inline RtVector2T<TemplateReal> operator/( const RtVector2T<TemplateReal>& v, TemplateReal f) {
        return v * (1/f);
    }

    template <typename TemplateReal>
    inline RtVector2T<TemplateReal> operator/( const RtVector2T<TemplateReal>& v,
            const RtVector2T<TemplateReal>& v2) {
        return RtVector2T<TemplateReal>(v.x / v2.x,v.y / v2.y);
    }
}