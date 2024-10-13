#ifndef VECTOR_3D_INCLUDE_H
#define VECTOR_3D_INCLUDE_H

#include <cmath>

namespace Retoccilus::Engine3D::ModelImporter {
    const double g_epsilon = 1e-6;

    template <typename Tp> class RtMIMatrix3x3_Template;

    template <typename Tp> class RtMIMatrix4x4_Template;

    template <typename Tp> class RtMIVector3D_Template {
    public:
        RtMIVector3D_Template() noexcept;

        RtMIVector3D_Template(Tp _x, Tp _y, Tp _z);

        explicit RtMIVector3D_Template(Tp _xyz);

        RtMIVector3D_Template(const RtMIVector3D_Template &rms) = default;

        const RtMIVector3D_Template &operator+=(const RtMIVector3D_Template &o);

        const RtMIVector3D_Template &operator-=(const RtMIVector3D_Template &o);

        const RtMIVector3D_Template &operator*=(Tp f);

        const RtMIVector3D_Template &operator/=(Tp f);

        RtMIVector3D_Template &operator*=(const RtMIMatrix3x3_Template<Tp> &mat);

        RtMIVector3D_Template &operator*=(const RtMIMatrix4x4_Template<Tp> &mat);

        Tp operator[](unsigned int i) const;

        Tp &operator[](unsigned int i);

        bool operator==(const RtMIVector3D_Template &other) const;

        bool operator!=(const RtMIVector3D_Template &other) const;

        bool operator<(const RtMIVector3D_Template &other) const;

        bool equal(const RtMIVector3D_Template &other, Tp epsilon = g_epsilon) const;

        template <typename TOther> operator RtMIVector3D_Template<TOther>() const;

        void set(Tp pX, Tp pY, Tp pZ);

        Tp squareLength() const;

        Tp length() const;

        RtMIVector3D_Template &normalize();

        RtMIVector3D_Template &normalizeSafe();

        const RtMIVector3D_Template symmetricMultiply(const RtMIVector3D_Template &o);

    public:
        Tp x;
        Tp y;
        Tp z;
    };
} // namespace Retoccilus::Engine3D::ModelImporter

#endif
