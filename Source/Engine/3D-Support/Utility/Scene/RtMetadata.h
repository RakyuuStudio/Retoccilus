#ifndef RT_METADATA_H
#define RT_METADATA_H

#include <cstdint>
#include <QString>
#include "Math/Quaternion.h"
#include "Math/Vector3.h"

namespace Retoccilus::Engine::ThreeDimensionalSupport::Scene {
    typedef enum rt_metadata_type {
        RT_MD_BOOL = 0, // For Boolean Metadata
        RT_MD_INT32 = 1, // For 32-bits Integer
        RT_MD_UINT64 = 2, // For Unsigned 64bit Integer
        RT_MD_FLOAT = 3,
        RT_MD_DOUBLE = 4,
        RT_MD_QSTRING = 5,
        RT_MD_RTVECTOR3D = 6,
        RT_MD_METADATA = 7,
        RT_MD_INT64 = 8,
        RT_MD_UINT32 = 9,
        RT_MD_METADATA_MAX = 10
    } rt_metadata_type;

    struct RtMetadataPair {
        rt_metadata_type type;
        void *metadata;

        RtMetadataPair() : type(RT_MD_METADATA_MAX), metadata(nullptr) {}
    };

    struct RtMetadata; // Forward Declaration
    inline rt_metadata_type GetRtMetadataType(const bool &) {
        return RT_MD_BOOL;
    }

    inline rt_metadata_type GetRtMetadataType(int32_t) {
        return RT_MD_INT32;
    }

    inline rt_metadata_type GetRtMetadataType(const uint64_t &) {
        return RT_MD_UINT64;
    }

    inline rt_metadata_type GetRtMetadataType(const float &) {
        return RT_MD_FLOAT;
    }

    inline rt_metadata_type GetRtMetadataType(const double &) {
        return RT_MD_DOUBLE;
    }

    inline rt_metadata_type GetRtMetadataType(const QString &) {
        return RT_MD_QSTRING;
    }

    inline rt_metadata_type GetRtMetadataType(const Utility::RtVector3T<rt_real> &) {
        return RT_MD_RTVECTOR3D;
    }

    inline rt_metadata_type GetRtMetadataType(const RtMetadata &) {
        return RT_MD_METADATA;
    }

    inline rt_metadata_type GetRtMetadataType(const int64_t &) {
        return RT_MD_INT64;
    }

    inline rt_metadata_type GetRtMetadataType(const uint32_t &) {
        return RT_MD_UINT32;
    }

    struct RtMetadata {
        unsigned int m_numberProperties;
        QString *m_keys;
        RtMetadataPair *m_values;

        RtMetadata() noexcept: m_numberProperties(0), m_keys(nullptr), m_values(nullptr) {}

        RtMetadata(const RtMetadata &rhs) : m_numberProperties(rhs.m_numberProperties), m_keys(nullptr),
                                            m_values(nullptr) {
            m_keys = new QString[m_numberProperties];
            for (auto i = 0; i < static_cast<size_t>(m_numberProperties); i++) {
                m_keys[i] = rhs.m_keys[i];
            }
            m_values = new RtMetadataPair[m_numberProperties];
            for (auto i = 0; i < static_cast<size_t>(m_numberProperties); i++) {
                m_values[i].type = rhs.m_values[i].type;
                switch (rhs.m_values[i].type) {
                    case RT_MD_BOOL: {
                        m_values[i].metadata = new bool();
                        ::memcpy(m_values[i].metadata, rhs.m_values[i].metadata, sizeof(bool));
                        break;
                    }
                    case RT_MD_INT32: {
                        int32_t tmp;
                        ::memcpy(&tmp, rhs.m_values[i].metadata, sizeof(int32_t));
                        m_values[i].metadata = new int32_t(tmp);
                        break;
                    }
                    case RT_MD_UINT64: {
                        uint64_t tmp;
                        ::memcpy(&tmp, rhs.m_values[i].metadata, sizeof(uint64_t));
                        m_values[i].metadata = new uint64_t(tmp);
                        break;
                    }
                    case RT_MD_FLOAT: {
                        float tmp;
                        ::memcpy(&tmp, rhs.m_values[i].metadata, sizeof(float));
                        m_values[i].metadata = new float(tmp);
                    }
                    case RT_MD_DOUBLE: {
                        double tmp;
                        ::memcpy(&tmp, rhs.m_values[i].metadata, sizeof(double));
                        m_values[i].metadata = new double(tmp);
                        break;
                    }
                    case RT_MD_QSTRING: {
                        QString tmp;
                        rhs.Get<QString>(static_cast<unsigned int>(i), tmp);
                        m_values[i].metadata = new QString(tmp);
                        break;
                    }
                    case RT_MD_RTVECTOR3D: {
                        Utility::RtVector3T<rt_real> tmp;
                        rhs.Get<Utility::RtVector3D>(static_cast<unsigned int>(i), tmp);
                        m_values[i].metadata = new Utility::RtVector3D(tmp);
                        break;
                    }
                    case RT_MD_METADATA: {
                        RtMetadata tmp;
                        rhs.Get<RtMetadata>(static_cast<unsigned int>(i), tmp);
                        m_values[i].metadata = new RtMetadata(tmp);
                    }
                    case RT_MD_INT64: {
                        int64_t tmp;
                        ::memcpy(&tmp, rhs.m_values[i].metadata, sizeof(int64_t));
                        m_values[i].metadata = new int64_t(tmp);
                        break;
                    }
                    case RT_MD_UINT32: {
                        uint32_t tmp;
                        ::memcpy(&tmp, rhs.m_values[i].metadata, sizeof(uint32_t));
                        m_values[i].metadata = new uint32_t(tmp);
                        break;
                    }
                    default: {
                        break;
                    }
                }
            }


        }
    };
}

#endif // RT_METADATA_H