#ifndef VECTOR_2_H
#define VECTOR_2_H

#include <cmath>
#include "../Definition.h"

namespace Retoccilus::Engine::ThreeDimensionalSupport::Utility {
    template<typename TemplateReal>
    class RtVector2T {
    public:
        explicit RtVector2T() : x(), y() {}
        explicit RtVector2T(TemplateReal _x, TemplateReal _y) : x(_x), y(_y) {}
        explicit RtVector2T(TemplateReal _xyzValue) : x(_xyzValue), y(_xyzValue) {}

        RtVector2T(const RtVector2T& op) = default;

        void Set(TemplateReal pX, TemplateReal pY);
        TemplateReal SquareLength() const ;
        TemplateReal Length() const ;
        RtVector2T& Normalize();

        const RtVector2T& operator += (const RtVector2T& o);
        const RtVector2T& operator -= (const RtVector2T& o);
        const RtVector2T& operator *= (TemplateReal f);
        const RtVector2T& operator /= (TemplateReal f);

        TemplateReal operator[](unsigned int i) const;

        bool operator== (const RtVector2T& other) const;
        bool operator!= (const RtVector2T& other) const;

        bool Equal(const RtVector2T &other, TemplateReal epsilon = rt_epsilon) const;

        RtVector2T& operator= (TemplateReal f);
        const RtVector2T SymmetricalMultiply(const RtVector2T &o);

        template<typename TpOther>
        explicit operator RtVector2T<TpOther> () const;

        TemplateReal x, y;
    };
}

#endif // VECTOR_2_H