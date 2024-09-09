#ifndef MATRIX_4_BY_4_H
#define MATRIX_4_BY_4_H

#include "Vector3.h"

namespace Retoccilus::Engine::ThreeDimensionalSupport::Utility {
    template<typename TemplateReal>
    class RtMatrix3x3T;

    template<typename TemplateReal>
    class RtQuaternionT;

    template<typename TemplateReal>
    class RtMatrix4x4T {
    public:
        RtMatrix4x4T() noexcept;
        RtMatrix4x4T(TemplateReal _a1, TemplateReal _a2, TemplateReal _a3, TemplateReal _a4,
                     TemplateReal _b1, TemplateReal _b2, TemplateReal _b3, TemplateReal _b4,
                     TemplateReal _c1, TemplateReal _c2, TemplateReal _c3, TemplateReal _c4,
                     TemplateReal _d1, TemplateReal _d2, TemplateReal _d3, TemplateReal _d4);

        explicit RtMatrix4x4T(const RtMatrix3x3T<TemplateReal> &m);
        explicit RtMatrix4x4T(const RtVector3T<TemplateReal> &scaling,
                              const RtQuaternionT<TemplateReal> &rotation,
                              const RtVector3T<TemplateReal> &position);

        TemplateReal *operator[](unsigned int pIndex);
        const TemplateReal *operator[](unsigned int pIndex) const;

        // Comparison Operators (==, !=)
        bool operator==(const RtMatrix4x4T<TemplateReal> &pMatrix) const;
        bool operator!=(const RtMatrix4x4T<TemplateReal> &pMatrix) const;

        bool Equal(const RtMatrix4x4T &m, TemplateReal epsilon = rt_epsilon) const;

        RtMatrix4x4T& operator*= (const RtMatrix4x4T& m);
        RtMatrix4x4T operator* (const RtMatrix4x4T& m) const;
        RtMatrix4x4T operator* (const TemplateReal& aFloat) const;
        RtMatrix4x4T operator+ (const RtMatrix4x4T& aMatrix) const;

        template <typename TemplateOther>
        explicit operator RtMatrix4x4T<TemplateOther> () const;

        RtMatrix4x4T& Transpose();
        RtMatrix4x4T& Inverse();
        TemplateReal Determinant() const;

        inline bool IsIdentity(const TemplateReal
                               epsilon = rt_identity_matrix_epsilon_default) const;

        void Decompose(RtVector3T<TemplateReal>& scaling, RtQuaternionT<TemplateReal>& rotation,
                        RtVector3T<TemplateReal>& position) const;

        void Decompose(RtVector3T<TemplateReal>& pScaling, RtVector3T<TemplateReal>& pRotation, 
                       RtVector3T<TemplateReal>& pPosition) const;
        void Decompose(RtVector3T<TemplateReal>& pScaling, RtVector3T<TemplateReal>& pRotationAxis,
                       TemplateReal& pRotationAngle, RtVector3T<TemplateReal>& pPosition) const;
        void DecomposeNoScaling(RtQuaternionT<TemplateReal>& rotation, RtVector3T<TemplateReal>& position) const;
        RtMatrix4x4T& FromEulerAnglesXYZ(TemplateReal x, TemplateReal y, TemplateReal z);
        RtMatrix4x4T& FromEulerAnglesXYZ(const RtVector3T<TemplateReal>& blubb);
        static RtMatrix4x4T& RotationX(TemplateReal a, RtMatrix4x4T& out);
        static RtMatrix4x4T& RotationY(TemplateReal a, RtMatrix4x4T& out);
        static RtMatrix4x4T& RotationZ(TemplateReal a, RtMatrix4x4T& out);
        static RtMatrix4x4T& Rotation(TemplateReal a, const RtVector3T<TemplateReal>& axis,
                                      RtMatrix4x4T& out);
        static RtMatrix4x4T& Translation( const RtVector3T<TemplateReal>& v,
                                          RtMatrix4x4T& out);
        static RtMatrix4x4T& Scaling( const RtVector3T<TemplateReal>& v, RtMatrix4x4T& out);
        static RtMatrix4x4T& FromToMatrix(const RtVector3T<TemplateReal>& from,
                                          const RtVector3T<TemplateReal>& to, RtMatrix4x4T& out);

    public:
        TemplateReal a1, a2, a3, a4;
        TemplateReal b1, b2, b3, b4;
        TemplateReal c1, c2, c3, c4;
        TemplateReal d1, d2, d3, d4;
    };
}

#endif