#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>
#include "../Definition.h"

namespace Retoccilus::Engine::ThreeDimensionalSupport::Utility {
    template<typename TemplateReal>
    class RtMatrix3x3T;

    template<typename TemplateReal>
    class RtMatrix4x4T;

    template<typename TemplateReal>
    class RtVector3T {
    public:
        RtVector3T() noexcept : x(), y(), z() {}
        RtVector3T(TemplateReal x, TemplateReal y, TemplateReal z) : x(x), y(y), z(z) {}
        explicit RtVector3T(TemplateReal _xyz) : x(_xyz), y(_xyz), z(_xyz) {}
        RtVector3T(const RtVector3T& o) = default;

        const RtVector3T& operator += (const RtVector3T& o);
        const RtVector3T& operator -= (const RtVector3T& o);
        const RtVector3T& operator *= (TemplateReal f);
        const RtVector3T& operator /= (TemplateReal f);
        RtVector3T& operator *= (const RtMatrix3x3T<TemplateReal>& mat);
        RtVector3T& operator *= (const RtMatrix4x4T<TemplateReal>& mat);
        TemplateReal operator[](unsigned int i) const;
        TemplateReal& operator[](unsigned int i);
        bool operator== (const RtVector3T& other) const;
        bool operator!= (const RtVector3T& other) const;
        bool operator< (const RtVector3T& other) const;
        [[maybe_unused]] bool Equal(const RtVector3T &other, TemplateReal epsilon = rt_epsilon) const;

        template <typename TemplateOther>
        explicit operator RtVector3T<TemplateOther> () const;

        void Set( TemplateReal pX, TemplateReal pY, TemplateReal pZ);
        TemplateReal SquareLength() const;
        TemplateReal Length() const;
        RtVector3T& Normalize();
        RtVector3T& NormalizeSafe();
        const RtVector3T SymmetricalMultiplication(const RtVector3T& o);

        TemplateReal operator*(const RtVector3T& o) const;
        RtVector3T operator^(const RtVector3T& o) const;
        RtVector3T operator+(const RtVector3T& o) const;
        RtVector3T operator-(const RtVector3T& o) const;
    public:
        TemplateReal x;
        TemplateReal y;
        TemplateReal z;
    };

    typedef RtVector3T<rt_real> RtVector3D;
    typedef RtVector3T<float> RtVector3f;
    typedef RtVector3T<double> RtVector3d;
}

#endif // VECTOR3_H