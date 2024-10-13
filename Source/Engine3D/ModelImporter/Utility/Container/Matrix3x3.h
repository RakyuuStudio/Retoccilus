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
     *
     * You shouldn't compare two float values by using `==`, `>=`, `>`, `<=`, `<`, `!=`. Instead, using
     * subtraction operation to compare the absolute result of the subtraction, if the result is equals
     * (or less than) the threshold you gave (in this case, it's 1e-6 (0.000001)), it will be considered as equal.
     *
     * For example, The absolute difference between two float variables is not less than g_epsilon
     * or equal to the g_epsilon. So in this case, two variables will NOT be considered as equal.
     *
     * @code
     * double a = 13.001, b = 114.514;
     * if (std::abs(a - b) < g_epsilon) {
     *     cout << "Equal";
     * } else {
     *     cout << "Not Equal";
     * }
     * @endcode
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
         * @brief Default constructor that initializes the matrix to empty.
         * @note This constructor is marked as `noexcept`, which means it will not throw any exception.
         */
        RtMIMatrix3x3_Template() noexcept;

        /**
         * @brief Default Constructor which allows you to initialize the matrix with nine elements (Row by Row).
         * @param _a1 - The element that in first row, first column.
         * @param _a2 - The element that in first row, second column.
         * @param _a3 - The element that in first row, third column.
         * @param _b1 - The element that in second row, first column.
         * @param _b2 - The element that in second row, second column.
         * @param _b3 - The element that in second row, third column.
         * @param _c1 - The element that in third row, first column.
         * @param _c2 - The element that in third row, second column.
         * @param _c3 - The element that in third row, third column.
         */
        RtMIMatrix3x3_Template(Tp _a1, Tp _a2, Tp _a3, Tp _b1, Tp _b2, Tp _b3, Tp _c1, Tp _c2, Tp _c3);

        /**
         * @brief Constructor which allows you to initialize the matrix by cutting a 4x4 matrix.
         * @param bMatrix - The 4x4 matrix to be cut.
         */
        explicit RtMIMatrix3x3_Template(const RtMIMatrix4x4_Template<Tp> &bMatrix);

        /**
         * @brief Overload the self-multiplication operator `*=` to multiply the current matrix with another matrix.
         * @param rhs - The right-hand side matrix to multiply with the current matrix.
         * @return Reference to the current matrix after multiplication.
         */
        RtMIMatrix3x3_Template &operator*=(const RtMIMatrix3x3_Template &rhs);

        /**
         * @brief Overload the multiplication operator `*` to multiply the current matrix with another matrix.
         * @param rhs - The right-hand side matrix to multiply with the current matrix.
         * @return A new matrix resulting from the multiplication.
         */
        RtMIMatrix3x3_Template operator*(const RtMIMatrix3x3_Template& rhs) const;

        /**
         * @brief Overload the subscript operator `[]` for non-constant access to the matrix elements.
         * @param index - The index of the element to access. Indexing is 0-based and row-major.
         * @return Pointer to the element at the given index.
         */
        Tp* operator[](unsigned int index);

        /**
         * @brief Overload the subscript operator `[]` for constant access to the matrix elements.
         * @param index - The index of the element to access. Indexing is 0-based and row-major.
         * @return Constant pointer to the element at the given index.
         */
        const Tp *operator[](unsigned int index) const;

        /**
         * @brief Overload the equality operator `==` to compare two matrices for equality.
         * @param matrix3X3Template - The matrix to compare with.
         * @return `true` if the matrices are equal, `false` otherwise.
         */
        bool operator==(const RtMIMatrix3x3_Template<Tp> &matrix3X3Template) const;

        /**
         * @brief Overload the inequality operator `!=` to compare two matrices for inequality.
         * @param matrix3X3Template - The matrix to compare with.
         * @return `true` if the matrices are not equal, `false` otherwise.
         */
        bool operator!=(const RtMIMatrix3x3_Template<Tp> &matrix3X3Template) const;

        /**
         * @brief Check if the current matrix is equal to another matrix within a given threshold.
         * @param m - The matrix to compare with.
         * @param epsilon - The threshold for equality comparison. Default is 1e-6 (0.000001).
         * @return `true` if the matrices are equal within the given threshold, `false` otherwise.
         *
         * @see `g_epsilon` variable.
         */
        bool equal(const RtMIMatrix3x3_Template<Tp> &m, Tp epsilon = g_epsilon) const;

        /**
         * @brief Convert the current matrix to another type of matrix.
         * @tparam TOther - The type of the matrix to convert to.
         * @return A new matrix of type TOther with the same values as the current matrix.
         */
        template <typename TOther> operator RtMIMatrix3x3_Template<TOther>() const;

        /**
         * @brief Transpose the current matrix.
         * @return Reference to the current matrix after transposition.
         */
        RtMIMatrix3x3_Template &transpose();

        /**
         * @brief Invert the current matrix.
         * @return Reference to the current matrix after inversion.
         */
        RtMIMatrix3x3_Template &inverse();

        /**
         * @brief Calculate the determinant of the current matrix.
         * @return The determinant of the current matrix.
         */
        Tp determinant() const;

        /**
         * @brief Rotate the current matrix around the Z-axis.
         * @param a - The angle of rotation in radians.
         * @param ot - The output matrix to store the result.
         * @return Reference to the current matrix after rotation.
         */
        static RtMIMatrix3x3_Template &rotationZ(Tp a, RtMIMatrix3x3_Template &ot);

        /**
         * @brief Rotate the current matrix around an arbitrary axis.
         * @param a - The angle of rotation in radians.
         * @param axis - The axis of rotation.
         * @param ot - The output matrix to store the result.
         * @return Reference to the current matrix after rotation.
         */
        static RtMIMatrix3x3_Template &rotation(Tp a, const RtMIVector3D_Template<Tp> &axis,
                                                RtMIMatrix3x3_Template &ot);

        /**
         * @brief Create a translation matrix.
         * @param v - The translation vector.
         * @param ot - The output matrix to store the result.
         * @return Reference to the current matrix after translation.
         */
        static RtMIMatrix3x3_Template &translation(const RtMIVector2D_Template<Tp> &v, RtMIMatrix3x3_Template &ot);

        /**
         * @brief Create a matrix that rotates a 3D vector from one direction to another.
         * @param from - The direction to rotate from.
         * @param to - The direction to rotate to.
         * @param ot - The output matrix to store the result.
         * @return Reference to the current matrix after rotation.
         */
        static RtMIMatrix3x3_Template &from3DVectorTo3DVector(const RtMIVector3D_Template<Tp> &from,
                                                              const RtMIVector3D_Template<Tp> &to,
                                                              RtMIMatrix3x3_Template &ot);

    public:
        /**
         * @brief The nine elements that used in the matrix.
         */
        Tp a1, a2, a3;
        Tp b1, b2, b3;
        Tp c1, c2, c3;
    };

    /**
     * @brief The alia used for the convenience.
     */
    using RtMIMatrix3x3 = RtMIMatrix3x3_Template<float>;
} // namespace Retoccilus::Engine3D::ModelImporter

#endif
