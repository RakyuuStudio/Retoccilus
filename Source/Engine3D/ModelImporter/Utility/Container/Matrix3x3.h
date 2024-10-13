// =========== Matrix3x3.h ============ Re-Implementation of Assimp Matrix3x3 =========== *- C++ -* //
// Original by Assimp Team, license under BSD 3-Clause License. Re-Implement by Rakyuu Studio.
// Copyright (c) 2023-2024 Rakyuu Studio, all rights reserved.
// SPDX-License-Identifier: LicenseRef-Apache-2.0-WITH-Retoccilus-Exception
// ================================================================================================ //

#ifndef MATRIX_3X3_INCLUDE_H
#define MATRIX_3X3_INCLUDE_H

#include <cmath>

namespace Retoccilus::Engine3D::ModelImporter {
    /**
     * @brief A float threshold to compare with 2 float values.
     */
    const double g_epsilon = 1e-6;

    /**
     * @brief Forward Declaration of class "RtMIMatrix4x4_Template" which declared in "Matrix4x4.h".
     * @tparam Tp - The type of the matrix elements.
     */
    template <typename Tp> class RtMIMatrix4x4_Template;

    /**
     * @brief Forward Declaration of class "RtMIVector2D_Template" which declared in "Vector2D.h".
     * @tparam Tp - The type of the vector elements.
     */
    template <typename Tp> class RtMIVector2D_Template;

    /**
     * @brief Forward Declaration of class "RtMIVector3D_Template" which declared in "Vector3D.h".
     * @tparam Tp - The type of the vector elements.
     */
    template <typename Tp> class RtMIVector3D_Template;

    /**
     * @brief Matrix3x3 Template Class.
     * @tparam Tp - The type of the matrix elements.
     */
    template <typename Tp> class RtMIMatrix3x3_Template {
    public:
        /**
         * @brief Default Constructor with 'noexcept' flag.
         */
        RtMIMatrix3x3_Template() noexcept;

        RtMIMatrix3x3_Template(Tp _a1, Tp _a2, Tp _a3, Tp _b1, Tp _b2, Tp _b3, Tp _c1, Tp _c2, Tp _c3);

        explicit RtMIMatrix3x3_Template(const RtMIMatrix4x4_Template<Tp> &bMatrix);

        RtMIMatrix3x3_Template &operator*=(const RtMIMatrix3x3_Template &rhs);

        RtMIMatrix3x3_Template operator*(const RtMIMatrix3x3_Template &rhs) const;

        Tp *operator[](unsigned int index);

        const Tp *operator[](unsigned int index) const;

        bool operator==(const RtMIMatrix3x3_Template<Tp> &matrix3X3Template) const;

        bool operator!=(const RtMIMatrix3x3_Template<Tp> &matrix3X3Template) const;

        bool equal(const RtMIMatrix3x3_Template<Tp> &m, Tp epsilon = g_epsilon) const;

        template <typename TOther> operator RtMIMatrix3x3_Template<TOther>() const;

        RtMIMatrix3x3_Template &transpose();

        RtMIMatrix3x3_Template &inverse();

        Tp determinant() const;

        static RtMIMatrix3x3_Template &rotationZ(Tp a, RtMIMatrix3x3_Template &ot);

        static RtMIMatrix3x3_Template &rotation(Tp a, const RtMIVector3D_Template<Tp> &axis,
                                                RtMIMatrix3x3_Template &ot);

        static RtMIMatrix3x3_Template &translation(const RtMIVector2D_Template<Tp> &v, RtMIMatrix3x3_Template &ot);

        static RtMIMatrix3x3_Template &from3DVectorTo3DVector(const RtMIVector3D_Template<Tp> &from,
                                                              const RtMIVector3D_Template<Tp> &to,
                                                              RtMIMatrix3x3_Template &ot);

    public:
        Tp a1, a2, a3;
        Tp b1, b2, b3;
        Tp c1, c2, c3;
    };

    using RtMIMatrix3x3 = RtMIMatrix3x3_Template<float>;
} // namespace Retoccilus::Engine3D::ModelImporter

#endif
