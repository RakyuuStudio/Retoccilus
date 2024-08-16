// =========--- Matrix3x3.inl ---============= Inline Implementation for a 3x3 matrix ====== -* C++ *- == //
// Copyright (c) 2023-2024 Rakyuu Studio and All Contributors. All rights reserved.
// This source code is licensed under BSD 3-Clause License. See LICENSE for more info.
// ====================================================================================================== //

#include <cmath>
#include <limits>
#include "Matrix3x3.h"
#include "Vector3.h"

namespace Retoccilus::Engine::ThreeDimensionalSupport::Utility {
    /**
     * @brief Constructs a 3x3 matrix from a 4x4 matrix.
     *
     * This constructor takes a 4x4 matrix and extracts the top-left 3x3 sub-matrix to create a new 3x3 matrix.
     * The 4x4 matrix is assumed to have the form:
     * <br>
     * | a1 a2 a3 a4 | <br>
     * | b1 b2 b3 b4 | <br>
     * | c1 c2 c3 c4 | <br>
     * | d1 d2 d3 d4 | <br>
     *
     * But we only need a 3x3 matrix. After the processing in this function, we have a 3x3 matrix like this: <br>
     *
     * | a1 a2 a3 | <br>
     * | b1 b2 b3 | <br>
     * | c1 c2 c3 | <br>
     *
     * @tparam TemplateReal - The type of matrix value.
     * @param matrix - The 4x4 matrix from which to extract the 3x3 sub-matrix.  <br>
     *
     * How to use this:
     * @code
     * RtMatrix4x4T<float> matrix = [Initialization]
     * RtMatrix3x3T<float> matrix3x3(matrix);
     * @endcode
     *
     * Why we do this? <br>
     * Extracting a 3x3 sub-matrix from a 4x4 matrix is a common operation,
     * especially when dealing with graphical transformations.
     * This is because many graphical transformations can be accomplished with only 3x3 matrices,
     * whereas 4x4 matrices are often used in chi-square coordinate systems to handle
     * translations and perspective projections.
    */
    template<typename TemplateReal>
    inline RtMatrix3x3T<TemplateReal>::RtMatrix3x3T(const RtMatrix4x4T<TemplateReal> &matrix) {
        a1 = matrix.a1;
        a2 = matrix.a2;
        a3 = matrix.a3;
        b1 = matrix.b1;
        b2 = matrix.b2;
        b3 = matrix.b3;
        c1 = matrix.c1;
        c2 = matrix.c2;
        c3 = matrix.c3;
    }

    /**
     * @brief Multiplies this matrix by another matrix.
     *
     * This function multiplies the current 3x3 matrix by another 3x3 matrix using the standard matrix multiplication algorithm.
     * The result is stored back into this matrix. <br>
     *
     * The mathematical formula for multiplying two 3x3 matrices M and N is: <br>
     *
     * | a1 a2 a3 |   | n11 n12 n13 |   | m11*n11 + m12*n21 + m13*n31  m11*n12 + m12*n22 + m13*n32  m11*n13 + m12*n23 + m13*n33 | <br>
     * | b1 b2 b3 | * | n21 n22 n23 | = | m21*n11 + m22*n21 + m23*n31  m21*n12 + m22*n22 + m23*n32  m21*n13 + m22*n23 + m23*n33 | <br>
     * | c1 c2 c3 |   | n31 n32 n33 |   | m31*n11 + m32*n21 + m33*n31  m31*n12 + m32*n22 + m33*n32  m31*n13 + m32*n23 + m33*n33 | <br>
     *
     * Where:
     * - m11, m12, m13 are elements of the first row of the current matrix. <br>
     * - m21, m22, m23 are elements of the second row of the current matrix. <br>
     * - m31, m32, m33 are elements of the third row of the current matrix. <br>
     * - n11, n12, n13 are elements of the first column of the other matrix. <br>
     * - n21, n22, n23 are elements of the second column of the other matrix. <br>
     * - n31, n32, n33 are elements of the third column of the other matrix. <br>
     *
     * @param m The matrix to multiply by.
     * @return A reference to this matrix after the multiplication.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrixA = [Matrix 3x3 - A];
     * RtMatrix3x3T<float> matrixB = [Matrix 4x4 - B];
     * matrixA *= matrixB;
     * @endcode
     *
     * Why we do this:
     * Matrix multiplication follows the rule in
     * linear algebra that the rows of the current matrix are multiplied by the columns of another matrix.
     */
    template<typename TemplateReal>
    inline RtMatrix3x3T<TemplateReal> &RtMatrix3x3T<TemplateReal>::operator*=(const RtMatrix3x3T<TemplateReal> &m) {
        *this = RtMatrix3x3T<TemplateReal>(
                m.a1 * a1 + m.b1 * a2 + m.c1 * a3,
                m.a2 * a1 + m.b2 * a2 + m.c2 * a3,
                m.a3 * a1 + m.b3 * a2 + m.c3 * a3,
                m.a1 * b1 + m.b1 * b2 + m.c1 * b3,
                m.a2 * b1 + m.b2 * b2 + m.c2 * b3,
                m.a3 * b1 + m.b3 * b2 + m.c3 * b3,
                m.a1 * c1 + m.b1 * c2 + m.c1 * c3,
                m.a2 * c1 + m.b2 * c2 + m.c2 * c3,
                m.a3 * c1 + m.b3 * c2 + m.c3 * c3
                );
        return *this;
    }

    /**
     * @brief Converts this matrix to another matrix type.
     *
     * This function converts the current matrix to a matrix of a different numeric type.
     * Each element of the matrix is cast to the new type.
     *
     * @tparam TemplateOther - The type of the matrix to convert to.
     * @return A new matrix of the converted type.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrixFloat = // Construct a matrix;
     * RtMatrix3x3T<double> matrixDouble = matrixFloat;  // Implicit conversion
     * @endcode
     *
     * Why we do this:
     * Converting between numeric types can be useful when interfacing with APIs that
     * require specific data types or when performing operations that benefit from higher precision.
    */
    template<typename TemplateReal>
    template<typename TemplateOther>
    RtMatrix3x3T<TemplateReal>::operator RtMatrix3x3T<TemplateOther>() const {
        return RtMatrix3x3T<TemplateOther>(
                static_cast<TemplateOther>(a1), static_cast<TemplateOther>(a2), static_cast<TemplateOther>(a3),
                static_cast<TemplateOther>(b1), static_cast<TemplateOther>(b2), static_cast<TemplateOther>(b3),
                static_cast<TemplateOther>(c1), static_cast<TemplateOther>(c2), static_cast<TemplateOther>(c3)
        );
    }

    /**
     * @brief Multiplies this matrix by another matrix.
     *
     * This function multiplies the current 3x3 matrix by another 3x3 matrix using the standard matrix multiplication algorithm.
     * The result is returned as a new matrix.
     *
     * The mathematical formula for multiplying two 3x3 matrices M and N is: <br>
     *
     * | a1 a2 a3 |   | n11 n12 n13 |   | m11*n11 + m12*n21 + m13*n31  m11*n12 + m12*n22 + m13*n32  m11*n13 + m12*n23 + m13*n33 | <br>
     * | b1 b2 b3 | * | n21 n22 n23 | = | m21*n11 + m22*n21 + m23*n31  m21*n12 + m22*n22 + m23*n32  m21*n13 + m22*n23 + m23*n33 | <br>
     * | c1 c2 c3 |   | n31 n32 n33 |   | m31*n11 + m32*n21 + m33*n31  m31*n12 + m32*n22 + m33*n32  m31*n13 + m32*n23 + m33*n33 | <br>
     *
     * Where:
     * - m11, m12, m13 are elements of the first row of the current matrix. <br>
     * - m21, m22, m23 are elements of the second row of the current matrix. <br>
     * - m31, m32, m33 are elements of the third row of the current matrix. <br>
     * - n11, n12, n13 are elements of the first column of the other matrix. <br>
     * - n21, n22, n23 are elements of the second column of the other matrix. <br>
     * - n31, n32, n33 are elements of the third column of the other matrix. <br>
     *
     * @param m - The matrix to multiply by.
     * @return A new matrix that is the result of the multiplication.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrixA = // Init Matrix;
     * RtMatrix3x3T<float> matrixB = // Init Matrix;
     * RtMatrix3x3T<float> result = matrixA * matrixB;
     * @endcode
     *
     * Why we do this:
     * Returning a new matrix allows for functional composition and avoids modifying the original matrix.
     */
    template<typename TemplateReal>
    inline RtMatrix3x3T<TemplateReal> RtMatrix3x3T<TemplateReal>::operator*(const RtMatrix3x3T<TemplateReal> &m) const {
        RtMatrix3x3T<TemplateReal> templ(*this);
        templ = templ * m;
        return templ;
    }

    /**
     * @brief Returns a pointer to the element at the specified index.
     *
     * This function returns a pointer to the element at the specified index.
     * The indices correspond to the rows of the matrix.
     *
     * @param p_iIndex - The index of the element to retrieve.
     * @return A pointer to the element at the specified index.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrix = // Initialize Matrix;
     * float *ptr = matrix[0];  // Get a pointer to the first row
     * @endcode
     *
     * Why we do this:
     * Providing access to the underlying elements of the matrix through
     * pointers can be useful for low-level manipulation or interfacing with C APIs.
    */
    template<typename TemplateReal>
    inline TemplateReal *RtMatrix3x3T<TemplateReal>::operator[](unsigned int p_iIndex) {
        if (p_iIndex == 0) {
            return &a1;
        } else if (p_iIndex == 1) {
            return &b1;
        } else if (p_iIndex == 2) {
            return &c1;
        }

        return &a1;
    }

    /**
     * @brief Returns a pointer to the element at the specified index.
     *
     * This function returns a pointer to the element at the specified index.
     * The indices correspond to the rows of the matrix.
     *
     * @param p_iIndex - The index of the element to retrieve.
     * @return A pointer to the element at the specified index.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrix = // Init 3x3 Matrix;
     * const float *ptr = matrix[0];  // Get a pointer to the first row
     * @endcode
     *
     * Why we do this:
     * Providing read-only access to the underlying elements of the matrix through pointers can
     * be useful for read-only operations or interfacing with C APIs.
    */
    template<typename TemplateReal>
    inline const TemplateReal *RtMatrix3x3T<TemplateReal>::operator[](unsigned int p_iIndex) const {
        if (p_iIndex == 0) {
            return &a1;
        } else if (p_iIndex == 1) {
            return &b1;
        } else if (p_iIndex == 2) {
            return &c1;
        }

        return &a1;
    }

    /**
     * @brief Checks if this matrix is equal to another matrix.
     *
     * This function checks if the current matrix is equal to another matrix.
     *
     * @param m The matrix to compare with.
     * @return True if the matrices are equal, false otherwise.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrixA = // Init Matrix;
     * RtMatrix3x3T<float> matrixB = // Init Matrix;
     * bool isEqual = matrixA == matrixB;
     * @endcode
     *
     * Why we do this:
     * Equality checks are fundamental in many algorithms and can help ensure the correctness of the program.
    */
    template<typename TemplateReal>
    inline bool RtMatrix3x3T<TemplateReal>::operator==(const RtMatrix3x3T<TemplateReal> &m) const {
        return a1 == m.a1 && a2 == m.a2 && a3 == m.a3 &&
               b1 == m.b1 && b2 == m.b2 && b3 == m.b3 &&
               c1 == m.c1 && c2 == m.c2 && c3 == m.c3;
    }

    /**
     * @brief Checks if this matrix is not equal to another matrix.
     *
     * This function checks if the current matrix is not equal to another matrix.
     *
     * @param m The matrix to compare with.
     * @return True if the matrices are not equal, false otherwise.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrixA = // Init Matrix;
     * RtMatrix3x3T<float> matrixB = // Init Matrix;
     * bool isNotEqual = matrixA != matrixB;
     * @endcode
     *
     * Why we do this:
     * Inequality checks are often needed to determine if two matrices differ in any way.
    */
    template<typename TemplateReal>
    inline bool RtMatrix3x3T<TemplateReal>::operator!=(const RtMatrix3x3T<TemplateReal> &m) const {
        return *this != m;
    }

    /**
     * @brief Checks if this matrix is approximately equal to another matrix within a given tolerance.
     *
     * This function checks if the current matrix is approximately equal to another matrix within a specified tolerance.
     * Two matrices are considered equal if the absolute difference between each corresponding element is less than
     * or equal to the tolerance.
     *
     * @param m The matrix to compare with.
     * @param epsilon The tolerance value for comparison.
     * @return True if the matrices are approximately equal, false otherwise.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrixA = // Init Matrix;
     * RtMatrix3x3T<float> matrixB = // Init Matrix;
     * bool isEqualApprox = matrixA.Equal(matrixB, 0.001f);  // Check equality within 0.001
     * @endcode
     *
     * Why we do this:
     * Approximate equality is important for floating-point numbers because exact equality
     * is rarely achieved due to rounding errors.
    */
    template<typename TemplateReal>
    inline bool RtMatrix3x3T<TemplateReal>::Equal(const RtMatrix3x3T<TemplateReal> &m, TemplateReal epsilon) const {
        return ((std::abs(a1 - m.a1) <= epsilon) && (std::abs(a2 - m.a2) <= epsilon) &&
                (std::abs(a3 - m.a3) <= epsilon) && (std::abs(b1 - m.b1) <= epsilon) &&
                (std::abs(b2 - m.b2) <= epsilon) && (std::abs(b3 - m.b3) <= epsilon) &&
                (std::abs(c1 - m.c1) <= epsilon) && (std::abs(c2 - m.c2) <= epsilon) &&
                (std::abs(c3 - m.c3) <= epsilon)
        );
    }

    /**
     * @brief Transposes this matrix.
     *
     * This function transposes the current 3x3 matrix, swapping rows and columns.
     *
     * The mathematical formula for transposing a 3x3 matrix M is: <br>
     *
     * | a1 b1 c1 |   | a1 a2 a3 | <br>
     * | a2 b2 c2 | => | b1 b2 b3 | <br>
     * | a3 b3 c3 |   | c1 c2 c3 | <br>
     *
     * Where:
     * - a1, b1, c1 are elements of the first column of the original matrix. <br>
     * - a2, b2, c2 are elements of the second column of the original matrix. <br>
     * - a3, b3, c3 are elements of the third column of the original matrix. <br>
     *
     * @return A reference to this matrix after transposition.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrix = // Init Matrix;
     * matrix.Transpose();  // Transpose the matrix
     * @endcode
     *
     * Why we do this:
     * Transposing a matrix is useful for operations that require the rows and columns to be swapped,
     * such as in certain transformations.
    */
    template<typename TemplateReal>
    inline RtMatrix3x3T<TemplateReal> &RtMatrix3x3T<TemplateReal>::Transpose() {
        std::swap(reinterpret_cast<TemplateReal&>(a2), reinterpret_cast<TemplateReal&>(b1));
        std::swap(reinterpret_cast<TemplateReal&>(a3), reinterpret_cast<TemplateReal&>(c1));
        std::swap(reinterpret_cast<TemplateReal&>(b3), reinterpret_cast<TemplateReal&>(c2));
    }

    /**
     * @brief Calculates the determinant of this matrix.
     *
     * This function calculates the determinant of the current 3x3 matrix.
     *
     * The mathematical formula for calculating the determinant of a 3x3 matrix M is: <br>
     *
     * det(M) = a1 * b2 * c3 - a1 * b3 * c2 + a2 * b3 * c1 - a2 * b1 * c3 + a3 * b1 * c2 - a3 * b2 * c1
     *
     * Where:
     * - a1, b1, c1 are elements of the first column of the matrix. <br>
     * - a2, b2, c2 are elements of the second column of the matrix. <br>
     * - a3, b3, c3 are elements of the third column of the matrix. <br>
     *
     * @return The determinant of the matrix.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrix = // Init Matrix;
     * float determinant = matrix.Determinant();  // Calculate the determinant
     * @endcode
     *
     * Why we do this:
     * The determinant of a matrix is a scalar value that can be used to determine whether the matrix is invertible and other properties of the matrix.
    */
    template<typename TemplateReal>
    inline TemplateReal RtMatrix3x3T<TemplateReal>::Determinant() const {
        return a1 * b2 * c3 - a1 * b3 * c2 + a2 * b3 * c1 - a2 * b1 * c3 + a3 * b1 * c2 - a3 * b2 * c1;
    }

    /**
     * @brief Inverts this matrix.
     *
     * This function inverts the current 3x3 matrix, if it is invertible.
     * If the matrix is singular (has a determinant of zero), the matrix is set to NaN values.
     *
     * The mathematical formula for inverting a 3x3 matrix M is: <br>
     *
     * M⁻¹ = (1/det(M)) * adj(M)
     *
     * Where:
     * - det(M) is the determinant of the matrix. <br>
     * - adj(M) is the adjugate (or adjoint) matrix, which is formed by taking the transpose
     * of the cofactor matrix. <br>
     *
     * @return A reference to this matrix after inversion.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrix = // Init Matrix;
     * matrix.Inverse();  // Invert the matrix
     * @endcode
     *
     * Why we do this:
     * Matrix inversion is crucial for solving systems of linear equations and other operations
     * that require the inverse of a matrix.
    */
    template<typename TemplateReal>
    inline RtMatrix3x3T<TemplateReal> &RtMatrix3x3T<TemplateReal>::Inverse() {
        TemplateReal determinant = Determinant();
        if (determinant == static_cast<TemplateReal>(0.0)) {
            const TemplateReal quiet_numeric_limit = std::numeric_limits<TemplateReal>::quiet_NaN();
            *this = RtMatrix3x3T<TemplateReal>(
                    quiet_numeric_limit, quiet_numeric_limit, quiet_numeric_limit,
                    quiet_numeric_limit, quiet_numeric_limit, quiet_numeric_limit,
                    quiet_numeric_limit, quiet_numeric_limit, quiet_numeric_limit
                    );

            return *this;
        }

        TemplateReal inversed_determinant = static_cast<TemplateReal>(1.0) / determinant;
        RtMatrix3x3T<TemplateReal> adjudicate_matrix;
        adjudicate_matrix.a1 = inversed_determinant * (b2 * c3 - b3 * c2);
        adjudicate_matrix.a2 = -inversed_determinant * (a2 * c3 - a3 * c2);
        adjudicate_matrix.a3 = inversed_determinant * (a2 * b3 - a3 * b2);
        adjudicate_matrix.b1 = -inversed_determinant * (b1 * c3 - b3 * c1);
        adjudicate_matrix.b2 = inversed_determinant * (a1 * c3 - a3 * c1);
        adjudicate_matrix.b3 = -inversed_determinant * (a1 * b3 - a3 * b1);
        adjudicate_matrix.c1 = inversed_determinant * (b1 * c2 - b2 * c1);
        adjudicate_matrix.c2 = -inversed_determinant * (a1 * c2 - a2 * c1);
        adjudicate_matrix.c3 = inversed_determinant * (a1 * b2 - a2 * b1);

        *this = adjudicate_matrix;
        return *this;
    }

    /**
     * @brief Creates a rotation matrix around the Z-axis.
     *
     * This function creates a 3x3 rotation matrix that rotates an object around the Z-axis by a specified angle.
     *
     * The mathematical formula for creating a rotation matrix around the Z-axis is: <br>
     *
     * | cos(a) -sin(a) 0 | <br>
     * | sin(a)  cos(a) 0 | <br>
     * | 0         0    1 | <br>
     *
     * Where:
     * - a is the angle of rotation. <br>
     *
     * @param a The angle of rotation around the Z-axis, in radians.
     * @param out The output matrix where the rotation matrix will be stored.
     * @return A reference to the output matrix.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrix;
     * RtMatrix3x3T<float> rotationMatrix;
     * matrix.RotationOnZ(M_PI / 2.0f, rotationMatrix);  // Rotate by 90 degrees around the Z-axis
     * @endcode
     *
     * Why we do this:
     * Rotation matrices are fundamental for transforming objects in 3D space,
     * especially for rotations around coordinate axes.
    */
    template<typename TemplateReal>
    inline RtMatrix3x3T<TemplateReal> &
    RtMatrix3x3T<TemplateReal>::RotationOnZ(TemplateReal a, RtMatrix3x3T<TemplateReal> &out) {
        out.a1 = std::cos(a);
        out.b2 = std::cos(a);
        out.a2 = -(out.b1);

        out.a3 = 0.0f;
        out.b3 = 0.0f;
        out.c1 = 0.0f;
        out.c2 = 0.0f;
        out.c3 = 1.0f;

        return out;
    }

    /**
     * @brief Creates a rotation matrix around an arbitrary axis.
     *
     * This function creates a 3x3 rotation matrix that rotates an object around an arbitrary axis by a specified angle.
     *
     * The mathematical formula for creating a rotation matrix around an arbitrary axis is: <br>
     *
     * | 1 - (1 - cos(a)) * (x * x)  (1 - cos(a)) * (x * y) - (sin(a) * z)  (1 - cos(a)) * (x * z) + (sin(a) * y) | <br>
     * | (1 - cos(a)) * (y * x) + (sin(a) * z)  1 - (1 - cos(a)) * (y * y)  (1 - cos(a)) * (y * z) - (sin(a) * x) | <br>
     * | (1 - cos(a)) * (z * x) - (sin(a) * y)  (1 - cos(a)) * (z * y) + (sin(a) * x)  1 - (1 - cos(a)) * (z * z) | <br>
     *
     * Where:
     * - a is the angle of rotation. <br>
     * - x, y, z are the components of the normalized rotation axis vector. <br>
     *
     * @param a The angle of rotation, in radians.
     * @param axis The normalized axis of rotation.
     * @param out The output matrix where the rotation matrix will be stored.
     * @return A reference to the output matrix.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrix;
     * RtMatrix3x3T<float> rotationMatrix;
     * RtVector3T<float> axis(1.0f, 0.0f, 0.0f);  // Axis of rotation
     * matrix.Rotation(M_PI / 2.0f, axis, rotationMatrix);  // Rotate by 90 degrees around the specified axis
     * @endcode
     *
     * Why we do this:
     * Arbitrary axis rotations are essential for more complex transformations in 3D graphics and physics simulations.
    */
    template<typename TemplateReal>
    inline RtMatrix3x3T<TemplateReal> &
    RtMatrix3x3T<TemplateReal>::Rotation(TemplateReal a, const RtVector3T<TemplateReal> &axis,
                                         RtMatrix3x3T<TemplateReal> &out) {
        TemplateReal cosineC = std::cos(a);
        TemplateReal cosineS = std::sin(a);
        TemplateReal tl = 1 - cosineC;
        TemplateReal ax = axis.x;
        TemplateReal ay = axis.y;
        TemplateReal az = axis.z;

        out.a1 = cosineC + tl * ax * ax;
        out.a2 = tl * ax * ay - cosineS * az;
        out.a3 = tl * ax * az + cosineS * ay;
        out.b1 = tl * ax * ay + cosineS * az;
        out.b2 = cosineC + tl * ay * ay;
        out.b3 = tl * ay * az - cosineS * ax;
        out.c1 = tl * ax * az - cosineS * ay;
        out.c2 = tl * ay * az + cosineS * ax;
        out.c3 = cosineC + tl * az * az;

        return out;
    }

    /**
     * @brief Creates a translation matrix.
     *
     * This function creates a 3x3 translation matrix that translates an object by a specified vector.
     *
     * Note: Since a 3x3 matrix cannot represent pure translations,
     * the resulting matrix will only include the translation component in the third column.
     *
     * @param v The translation vector.
     * @param out The output matrix where the translation matrix will be stored.
     * @return A reference to the output matrix.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrix;
     * RtMatrix3x3T<float> translationMatrix;
     * RtVector2T<float> v(10.0f, 5.0f);  // Translation vector
     * matrix.Translation(v, translationMatrix);  // Translate by the specified vector
     * @endcode
     *
     * Why we do this:
     * While 3x3 matrices cannot represent pure translations, this function provides a
     * simplified version for certain applications.
    */
    template<typename TemplateReal>
    inline RtMatrix3x3T<TemplateReal> &
    RtMatrix3x3T<TemplateReal>::Translation(const RtVector2T<TemplateReal> &v, RtMatrix3x3T<TemplateReal> &out) {
        out = RtMatrix3x3T<TemplateReal>();
        out.a3 = v.x;
        out.b3 = v.y;

        return out;
    }

    /**
     * @brief Creates a rotation matrix to align one vector to another.
     *
     * This function creates a 3x3 rotation matrix that rotates one vector to align with another.
     *
     * The mathematical formula for creating a rotation matrix to align one vector to another involves the cross product and dot product of the vectors.
     *
     * @param from The source vector.
     * @param to The target vector.
     * @param out The output matrix where the rotation matrix will be stored.
     * @return A reference to the output matrix.
     *
     * How to use this:  <br>
     * @code
     * RtMatrix3x3T<float> matrix;
     * RtMatrix3x3T<float> alignmentMatrix;
     * RtVector3T<float> from(1.0f, 0.0f, 0.0f);  // Source vector
     * RtVector3T<float> to(0.0f, 1.0f, 0.0f);  // Target vector
     * matrix.FromToMatrix(from, to, alignmentMatrix);  // Align the source vector to the target vector
     * @endcode
     *
     * Why we do this:
     * Aligning one vector to another is useful for orienting objects in 3D space, such as aiming a camera or orienting a model.
    */
    template<typename TemplateReal>
    inline RtMatrix3x3T<TemplateReal> &
    RtMatrix3x3T<TemplateReal>::FromToMatrix(const RtVector3T<TemplateReal> &from, const RtVector3T<TemplateReal> &to,
                                             RtMatrix3x3T<TemplateReal> &out) {
        const TemplateReal e = from * to;
        const TemplateReal f = (e < 0) ? -e : e;

        if (f > static_cast<TemplateReal>(1.0) - static_cast<TemplateReal>(0.00001)) {
            RtVector3D u, v, x;

            x.x = (from.x > 0.0) ? from.x : -from.z;
            x.y = (from.y > 0.0) ? from.y : -from.y;
            x.z = (from.z > 0.0) ? from.z : -from.x;

            if (x.x < x.y) {
                if (x.x < x.z) {
                    x.x = static_cast<TemplateReal>(1.0);
                    x.y = static_cast<TemplateReal>(0.0);
                    x.z = static_cast<TemplateReal>(0.0);
                } else {
                    x.x = static_cast<TemplateReal>(0.0);
                    x.y = static_cast<TemplateReal>(0.0);
                    x.z = static_cast<TemplateReal>(1.0);
                }
            } else {
                if (x.y < x.z) {
                    x.x = static_cast<TemplateReal>(0.0);
                    x.y = static_cast<TemplateReal>(1.0);
                    x.z = static_cast<TemplateReal>(0.0);
                } else {
                    x.x = static_cast<TemplateReal>(0.0);
                    x.y = static_cast<TemplateReal>(0.0);
                    x.z = static_cast<TemplateReal>(1.0);
                }
            }

            u.x = x.x - from.x;
            u.y = x.y - from.y;
            u.z = x.z - from.z;

            v.x = x.x - to.x;
            v.y = x.y - to.y;
            v.z = x.z - to.z;

            const TemplateReal _c1v = static_cast<TemplateReal>(2.0) / (u * u);
            const TemplateReal _c2v = static_cast<TemplateReal>(2.0) / (v * v);
            const TemplateReal _c3v = _c1v * _c2v * (u * v);

            for (unsigned int i = 0; i < 3; i++) {
                for (unsigned int j = 0; j < 3; j++) {
                    out[i][j] = -_c1v * u[i] * u[j] - _c2v * v[i] * v[j] + _c3v * v[i] * u[j];
                }
                out[i][i] += static_cast<TemplateReal>(1.0);
            }
        } else {
            const RtVector3D v = from ^ to;
            const TemplateReal h = static_cast<TemplateReal>(1.0) / (static_cast<TemplateReal>(1.0) + e);
            const TemplateReal h_x = h * v.x;
            const TemplateReal h_z = h * v.z;
            const TemplateReal h_xy = h_x * v.y;
            const TemplateReal h_yz = h_z * v.y;
            const TemplateReal h_xz = h_x * v.z;

            out[0][0] = e + h_x * v.x;
            out[0][1] = h_xy - v.z;
            out[0][2] = h_xz + v.y;

            out[1][0] = h_xy + v.z;
            out[1][1] = e + h * v.y * v.y;
            out[1][2] = h_yz - v.x;

            out[2][0] = h_xz - v.y;
            out[2][1] = h_yz + v.x;
            out[2][2] = e + h_z * v.z;
        }

        return out;
    }
}