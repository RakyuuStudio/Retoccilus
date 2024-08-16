// =========--- Matrix3x3.h ---====================== Implementation for a 3x3 matrix ====== -* C++ *- == //
// Copyright (c) 2023-2024 Rakyuu Studio and All Contributors. All rights reserved.
// This source code is licensed under BSD 3-Clause License. See LICENSE for more info.
// ====================================================================================================== //

/**
 * @file Matrix3x3.h
 * @brief A 3x3 matrix implementation.
 */
#ifndef MATRIX_3x3_H
#define MATRIX_3x3_H

#include "../Definition.h"

namespace Retoccilus::Engine::ThreeDimensionalSupport::Utility {
    template<typename Tp>
    class RtMatrix4x4T;

    template<typename Tp>
    class RtVector2T;

    template<typename Tp>
    class RtVector3T;

    /**
     * @brief RtMatrix3x3T class provides a 3x3 matrix with operations.
     * This class represents a 3x3 matrix and provides some simple and commonly used operations
     * for matrix operation. Such as dot multiplication and etc.
     * @tparam TemplateReal - A Template Parameter for each type of value in Matrix.
     */
    template<typename TemplateReal>
    class RtMatrix3x3T {
    public:
        /**
         * @brief Constructor.
         * Construct a 3x3 matrix with 3 default values used in the default matrix.
         */
        explicit RtMatrix3x3T() noexcept : a1(static_cast<TemplateReal>(1.0f)), a2(), a3(),
                                           b1(), b2(static_cast<TemplateReal>(1.0f)), b3(),
                                           c1(), c2(), c3(static_cast<TemplateReal>(1.0f))
        {}

        /**
         * @brief Constructor.
         * Construct a 3x3 matrix with 9 parameters. Each parameter is a value of the matrix position.
         *
         * @param _a1 - Line1 Column1 Value
         * @param _a2 - Line1 Column2 Value
         * @param _a3 - Line1 Column3 Value
         * @param _b1 - Line2 Column1 Value
         * @param _b2 - Line2 Column2 Value
         * @param _b3 - Line2 Column3 Value
         * @param _c1 - Line3 Column1 Value
         * @param _c2 - Line3 Column2 Value
         * @param _c3 - Line3 Column3 Value
         */
        explicit RtMatrix3x3T(TemplateReal _a1, TemplateReal _a2, TemplateReal _a3,
                              TemplateReal _b1, TemplateReal _b2, TemplateReal _b3,
                              TemplateReal _c1, TemplateReal _c2, TemplateReal _c3)
                              : a1(_a1), a2(_a2), a3(_a3),
                               b1(_b1), b2(_b2), b3(_b3),
                               c1(_c1), c2(_c2), c3(_c3)
        {}

        /**
         * @brief Constructor.
         * This constructor accepts a 4x4 matrix instance to construct a 3x3 matrix.
         * @param matrix - A [RtMatrix4x4T] Object.
         */
        explicit RtMatrix3x3T(const RtMatrix4x4T<TemplateReal> &matrix);

        /**
         * @brief Multiplies this matrix by another matrix.
         * @param m - The matrix to multiply by.
         * @return A reference to this matrix after the multiplication.
         */
        RtMatrix3x3T &operator *= (const RtMatrix3x3T& m);

        /**
         * @brief Multiplies this matrix by another matrix.
         * @param m - The matrix to multiply by.
         * @return A new matrix that is the result of the multiplication.
         */
        RtMatrix3x3T operator  * (const RtMatrix3x3T& m) const;

        /**
         * @brief Returns a pointer to the element at the specified index.
         * @param p_iIndex - The index of the element to retrieve.
         * @return A pointer to the element at the specified index.
         */
        TemplateReal *operator[] (unsigned int p_iIndex);

        /**
         * @brief Returns a pointer to the element at the specified index.
         * @param p_iIndex - The index of the element to retrieve.
         * @return A pointer to the element at the specified index.
         */
        const TemplateReal *operator[] (unsigned int p_iIndex) const;

        /**
         * @brief Checks if this matrix is equal to another matrix.
         * @param m - The matrix to compare with.
         * @return True if the matrices are equal, false otherwise.
         */
        bool operator==(const RtMatrix3x3T<TemplateReal> &m) const;

        /**
         * @brief Checks if this matrix is not equal to another matrix.
         * @param m - The matrix to compare with.
         * @return True if the matrices are not equal, false otherwise.
         */
        bool operator!=(const RtMatrix3x3T<TemplateReal> &m) const;

        /**
         * @brief Checks if this matrix is approximately equal to another matrix within a given tolerance.
         * @param m - The matrix to compare with.
         * @param epsilon - The tolerance value for comparison. Declared in Definition.h
         * @return True if the matrices are approximately equal, false otherwise.
         */
        bool Equal(const RtMatrix3x3T<TemplateReal> &m, TemplateReal epsilon = rt_epsilon) const;

        /**
         * @brief Converts this matrix to another matrix type.
         * @return A new matrix of the converted type.
         */
        template<typename TemplateOther>
        explicit operator RtMatrix3x3T<TemplateOther>() const;

        /**
         * @brief Transposes this matrix.
         * @return A reference to this matrix after transposition.
         */
        RtMatrix3x3T &Transpose();

        /**
         * @brief Inverts this matrix.
         * @return A reference to this matrix after inversion.
         */
        RtMatrix3x3T &Inverse();

        /**
         * @brief Calculates the determinant of this matrix.
         * @return The determinant of the matrix.
         */
        TemplateReal Determinant() const;

        /**
         * @brief Creates a rotation matrix around the Z axis.
         * @param a - The angle of rotation in radians.
         * @param out - The output matrix where the result will be stored.
         * @return A reference to the output matrix.
         */
        static RtMatrix3x3T &RotationOnZ(TemplateReal a, RtMatrix3x3T &out);

        /**
         * @brief Creates a rotation matrix around an arbitrary axis.
         * @param a - The angle of rotation in radians.
         * @param axis - The axis of rotation.
         * @param out - The output matrix where the result will be stored.
         * @return A reference to the output matrix.
         */
        static RtMatrix3x3T &Rotation(TemplateReal a, const RtVector3T<TemplateReal> &axis, RtMatrix3x3T &out);

        /**
         * @brief Creates a translation matrix.
         * @param v - The translation vector.
         * @param out - The output matrix where the result will be stored.
         * @return A reference to the output matrix.
         */
        static RtMatrix3x3T &Translation(const RtVector2T<TemplateReal> &v, RtMatrix3x3T &out);

        /**
         * @brief Creates a matrix that transforms one vector into another.
         * @param from - The source vector.
         * @param to - The target vector.
         * @param out - The output matrix where the result will be stored.
         * @return A reference to the output matrix.
         */
        static RtMatrix3x3T& FromToMatrix(const RtVector3T<TemplateReal>& from,
                                          const RtVector3T<TemplateReal>& to, RtMatrix3x3T& out);
        
    public:
        TemplateReal a1, a2, a3;
        TemplateReal b1, b2, b3;
        TemplateReal c1, c2, c3;
    };
}

#endif // MATRIX_3x3_H