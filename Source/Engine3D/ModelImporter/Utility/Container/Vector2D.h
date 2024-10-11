#ifndef VECTOR_2D_INCLUDE_H
#define VECTOR_2D_INCLUDE_H

#include <cmath>

namespace Retoccilus::Engine3D::ModelImporter {
    const double g_epsilon = 1e-6;

    template <typename Tp>
    class RtMIVector2D_Template {
    public:
        RtMIVector2D_Template ();
        RtMIVector2D_Template (Tp _x, Tp _y);
        explicit RtMIVector2D_Template (Tp _xyz);
        RtMIVector2D_Template(const RtMIVector2D_Template& o) = default;

        void setData(Tp _X, Tp _Y);
        Tp squareLength() const;
        Tp length();
        RtMIVector2D_Template &normalize();

        const RtMIVector2D_Template& operator+= (const RtMIVector2D_Template& o);
        const RtMIVector2D_Template& operator-= (const RtMIVector2D_Template& o);
        const RtMIVector2D_Template& operator*= (Tp f);
        const RtMIVector2D_Template& operator/= (Tp f);

        Tp operator[](unsigned int i) const;

        bool operator== (const RtMIVector2D_Template& other) const;
        bool operator!= (const RtMIVector2D_Template& other) const;

        bool equal(const RtMIVector2D_Template &other, Tp epsilon = g_epsilon) const;

        RtMIVector2D_Template& operator= (Tp f);
        const RtMIVector2D_Template symmetricMultiply(const RtMIVector2D_Template& o);

        template <typename TOther>
        operator RtMIVector2D_Template<TOther> () const;

        Tp x, y;
    };

    using RtMIVector2D = RtMIVector2D_Template<float>;
}

#endif
