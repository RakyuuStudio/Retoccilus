// =========--- Quaternion.h ---===================== Implementation for Quaternion ======== -* C++ *- == //
// Copyright (c) 2023-2024 Rakyuu Studio and All Contributors. All rights reserved.
// This source code is licensed under BSD 3-Clause License. See LICENSE for more info.
// ====================================================================================================== //

/**
 * @file Quaternion.h
 * @brief A Quaternion Implementation
*/

#ifndef QUATERNION_H
#define QUATERNION_H

#include "../Definition.h"

namespace Retoccilus::Engine::ThreeDimensionalSupport::Utility {
    template<typename TemplateReal>
    class RtVector3T;

    template<typename TemplateReal>
    class RtMatrix3x3T;

    template<typename TemplateReal> class
    RtMatrix4x4T;

    template <typename TemplateReal>
    class RtQuaternionT {
    public:
        RtQuaternionT() noexcept : w(1.0), x(), y(), z()
        {}

        RtQuaternionT(TemplateReal pw, TemplateReal px, TemplateReal py, TemplateReal pz)
        : w(pw), x(px), y(py), z(pz)
        {}

        [[maybe_unused]] explicit RtQuaternionT(const RtMatrix3x3T<TemplateReal>& pRotMatrix);
        RtQuaternionT( TemplateReal roty, TemplateReal rotz, TemplateReal rotx);
        RtQuaternionT( RtVector3T<TemplateReal> axis, TemplateReal angle);
        explicit RtQuaternionT( RtVector3T<TemplateReal> normalized);
        RtMatrix3x3T<TemplateReal> GetMatrix() const;
        bool operator== (const RtQuaternionT& o) const;
        bool operator!= (const RtQuaternionT& o) const;
        RtQuaternionT& operator *= (const RtMatrix4x4T<TemplateReal>& mat);
        bool Equal(const RtQuaternionT &o, TemplateReal epsilon = rt_epsilon) const;
        RtQuaternionT& Normalize();
        RtQuaternionT& Conjugate();
        RtVector3T<TemplateReal> Rotate(const RtVector3T<TemplateReal>& in) const;
        RtQuaternionT operator * (const RtQuaternionT& two) const;
        static void Interpolate( RtQuaternionT& pOut, const RtQuaternionT& pStart,
                                 const RtQuaternionT& pEnd, TemplateReal pFactor);
    public:
        TemplateReal w;
        TemplateReal x;
        TemplateReal y;
        TemplateReal z;
    };
}

#endif // QUATERNION_H