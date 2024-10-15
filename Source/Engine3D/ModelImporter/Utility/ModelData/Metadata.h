// =========== Metadata.h ============= Re-Implementation of Assimp Model Metadata ====== *- C++ -*
// // Original by Assimp Team, license under BSD 3-Clause License. Re-Implement by Rakyuu Studio.
// Copyright (c) 2023-2024 Rakyuu Studio, all rights reserved.
// SPDX-License-Identifier: LicenseRef-Apache-2.0-WITH-Retoccilus-Exception
// ================================================================================================
// //

#include "ModelImporter/Utility/Container/Quaternion.h"
#include "ModelImporter/Utility/Container/Vector3D.h"
#include "ModelImporter/Utility/Tools/RetoMIString.h"
#include <cstdint>
#include <cstring>
#include <type_traits>

namespace Retoccilus::Engine3D::ModelImporter {
    enum k_RtMIMetadataType {
        Rt_Bool,
        Rt_Int32,
        Rt_UnsignedInt64,
        Rt_Float,
        Rt_Double,
        Rt_RTMIString,
        Rt_RTMIVector3D,
        Rt_RTMIMetadata,
        Rt_Int64,
        Rt_UnsignedInt32,
        Rt_Metadata_Max
    };

    using RtMIMetadataType = k_RtMIMetadataType;

    struct RtMIMetadataPair {
        RtMIMetadataType m_type;
        void *m_data;

        RtMIMetadataPair() : m_type(RtMIMetadataType::Rt_Metadata_Max), m_data(nullptr) {
        }
    };

    struct RtMIMetadata;

    // TODO: This function may lead to wrong result.
    //      Me from the future (or someone from the contributor team) should pay more attention on
    //      this.
    /*
     * Usage:
     * auto type = GetRtMIMetadataType<bool>(); (The template parameter you entered will decide the
     * return metadata type)
     */
    //    template <typename Tp> constexpr RtMIMetadataType GetRtMIMetadataType() {
    //        if constexpr (std::is_same_v<Tp, bool>)
    //            return RtMIMetadataType::Rt_Bool;
    //        else if constexpr (std::is_same_v<Tp, int32_t>)
    //            return RtMIMetadataType::Rt_Int32;
    //        else if constexpr (std::is_same_v<Tp, uint64_t>)
    //            return RtMIMetadataType::Rt_UnsignedInt64;
    //        else if constexpr (std::is_same_v<Tp, float>)
    //            return RtMIMetadataType::Rt_Float;
    //        else if constexpr (std::is_same_v<Tp, double>)
    //            return RtMIMetadataType::Rt_Double;
    //        else if constexpr (std::is_same_v<Tp, RetoMIString>)
    //            return RtMIMetadataType::Rt_RTMIString;
    //        else if constexpr (std::is_same_v<Tp, RtMIVector3D_Template<float>>)
    //            return RtMIMetadataType::Rt_RTMIVector3D;
    //        else if constexpr (std::is_same_v<Tp, RtMIMetadata>)
    //            return RtMIMetadataType::Rt_RTMIMetadata;
    //        else if constexpr (std::is_same_v<Tp, int64_t>)
    //            return RtMIMetadataType::Rt_Int64;
    //        else if constexpr (std::is_same_v<Tp, uint32_t>)
    //            return RtMIMetadataType::Rt_UnsignedInt32;
    //    }

    inline RtMIMetadataType GetRtMetadataType(const bool &) {
        return Rt_Bool;
    }
    inline RtMIMetadataType GetRtMetadataType(int32_t) {
        return Rt_Int32;
    }
    inline RtMIMetadataType GetRtMetadataType(const uint64_t &) {
        return Rt_UnsignedInt64;
    }
    inline RtMIMetadataType GetRtMetadataType(const float &) {
        return Rt_Float;
    }
    inline RtMIMetadataType GetRtMetadataType(const double &) {
        return Rt_Double;
    }
    inline RtMIMetadataType GetRtMetadataType(const RetoMIString &) {
        return Rt_RTMIString;
    }
    inline RtMIMetadataType GetRtMetadataType(const RtMIVector3D_Template<float> &) {
        return Rt_RTMIVector3D;
    }
    inline RtMIMetadataType GetRtMetadataType(const RtMIMetadata &) {
        return Rt_RTMIMetadata;
    }
    inline RtMIMetadataType GetRtMetadataType(const int64_t &) {
        return Rt_Int64;
    }
    inline RtMIMetadataType GetRtMetadataType(const uint32_t &) {
        return Rt_UnsignedInt32;
    }

    // TODO: This constructor may lead to wrong result.
    //  Me from the future (or someone from the contributor team) should pay more attention on this.
    struct RtMIMetadata {
        unsigned int
            m_numProperties; // How many pairs of RtMIMetadataPair are in this metadata object?
        RetoMIString *m_keys;
        RtMIMetadataPair *m_values;

        RtMIMetadata() noexcept : m_numProperties(0), m_keys(nullptr), m_values(nullptr) {
        }

        RtMIMetadata(const RtMIMetadata &rhs)
            : m_numProperties(rhs.m_numProperties),
              m_keys(nullptr),
              m_values(nullptr) {
            m_keys = new RetoMIString[m_numProperties];
            for (auto i = 0; i < static_cast<size_t>(m_numProperties); ++i) {
                m_keys[i] = rhs.m_keys[i];
            }

            m_values = new RtMIMetadataPair[m_numProperties];
            for (size_t i = 0; i < static_cast<size_t>(m_numProperties); ++i) {
                m_values[i].m_type = rhs.m_values[i].m_type;
                switch (rhs.m_values[i].m_type) {
                case k_RtMIMetadataType::Rt_Bool: {
                    m_values[i].m_data = new bool;
#if defined(_MSC_VER)
                    ::memcpy_s(m_values[i].m_data, sizeof(bool), rhs.m_values[i].m_data,
                               sizeof(bool));
#else
                    ::memcpy(m_values[i].m_data, rhs.m_values[i].m_data, sizeof(bool));
#endif
                    break;
                }
                case k_RtMIMetadataType::Rt_Int32: {
                    int32_t v;
#if defined(_MSC_VER)
                    ::memcpy_s(&v, sizeof(int32_t), rhs.m_values[i].m_data, sizeof(int32_t));
#else
                    ::memcpy(&v, rhs.m_values[i].m_data, sizeof(int32_t));
#endif
                    m_values[i].m_data = new int32_t(v);
                } break;
                case k_RtMIMetadataType::Rt_UnsignedInt64: {
                    uint64_t v;
#if defined(_MSC_VER)
                    ::memcpy_s(&v, sizeof(uint64_t), rhs.m_values[i].m_data, sizeof(uint64_t));
#else
                    ::memcpy(&v, rhs.m_values[i].m_data, sizeof(uint64_t));
#endif
                    m_values[i].m_data = new uint64_t(v);
                } break;
                case k_RtMIMetadataType::Rt_Float: {
                    float v;
#if defined(_MSC_VER)
                    ::memcpy_s(&v, sizeof(float), rhs.m_values[i].m_data, sizeof(float));
#else
                    ::memcpy(&v, rhs.m_values[i].m_data, sizeof(float));
#endif
                    m_values[i].m_data = new float(v);
                } break;
                case k_RtMIMetadataType::Rt_Double: {
                    double v;
#if defined(_MSC_VER)
                    ::memcpy_s(&v, sizeof(double), rhs.m_values[i].m_data, sizeof(double));
#else
                    ::memcpy(&v, rhs.m_values[i].m_data, sizeof(double));
#endif
                    m_values[i].m_data = new double(v);
                } break;
                case k_RtMIMetadataType::Rt_RTMIString: {
                    RetoMIString v;
                    rhs.get<RetoMIString>(static_cast<unsigned int>(i), v);
                    m_values[i].m_data = new RetoMIString(v);
                } break;
                case k_RtMIMetadataType::Rt_RTMIVector3D: {
                    RtMIVector3D_Template<float> v;
                    rhs.get<RtMIVector3D_Template<float>>(static_cast<unsigned int>(i), v);
                    m_values[i].m_data = new RtMIVector3D_Template<float>(v);
                } break;
                case k_RtMIMetadataType::Rt_RTMIMetadata: {
                    RtMIMetadata v;
                    rhs.get<RtMIMetadata>(static_cast<unsigned int>(i), v);
                    m_values[i].m_data = new RtMIMetadata(v);
                } break;
                case k_RtMIMetadataType::Rt_Int64: {
                    int64_t v;
#if defined(_MSC_VER)
                    ::memcpy_s(&v, sizeof(int64_t), rhs.m_values[i].m_data, sizeof(int64_t));
#else
                    ::memcpy(&v, rhs.m_values[i].m_data, sizeof(int64_t));
#endif
                    m_values[i].m_data = new int64_t(v);
                } break;
                case k_RtMIMetadataType::Rt_UnsignedInt32: {
                    uint32_t v;
#if defined(_MSC_VER)
                    ::memcpy_s(&v, sizeof(uint32_t), rhs.m_values[i].m_data, sizeof(uint32_t));
#else
                    ::memcpy(&v, rhs.m_values[i].m_data, sizeof(uint32_t));
#endif
                    m_values[i].m_data = new uint32_t(v);
                } break;
                default:
                    break;
                }
            }
        }

        RtMIMetadata &operator=(RtMIMetadata rhs) {
            using std::swap;
            swap(m_numProperties, rhs.m_numProperties);
            swap(m_keys, rhs.m_keys);
            swap(m_values, rhs.m_values);
            return *this;
        }

        ~RtMIMetadata() {
            delete[] m_keys;
            m_keys = nullptr;

            if (m_values) {
                for (auto i = 0; i < m_numProperties; i++) {
                    void *data = m_values[i].m_data;
                    switch (m_values[i].m_type) {
                    case k_RtMIMetadataType::Rt_Bool:
                        delete static_cast<bool *>(data);
                        break;
                    case k_RtMIMetadataType::Rt_Int32:
                        delete static_cast<int32_t *>(data);
                        break;
                    case k_RtMIMetadataType::Rt_UnsignedInt64:
                        delete static_cast<uint64_t *>(data);
                        break;
                    case k_RtMIMetadataType::Rt_Float:
                        delete static_cast<float *>(data);
                        break;
                    case k_RtMIMetadataType::Rt_Double:
                        delete static_cast<double *>(data);
                        break;
                    case k_RtMIMetadataType::Rt_RTMIString:
                        delete static_cast<RetoMIString *>(data);
                        break;
                    case k_RtMIMetadataType::Rt_RTMIVector3D:
                        delete static_cast<RtMIVector3D_Template<float> *>(data);
                        break;
                    case k_RtMIMetadataType::Rt_RTMIMetadata:
                        delete static_cast<RtMIMetadata *>(data);
                        break;
                    case k_RtMIMetadataType::Rt_Int64:
                        delete static_cast<int64_t *>(data);
                        break;
                    case k_RtMIMetadataType::Rt_UnsignedInt32:
                        delete static_cast<uint32_t *>(data);
                        break;
                    case k_RtMIMetadataType::Rt_Metadata_Max:
                    default:
                        break;
                    }
                }

                delete[] m_values;
                m_values = nullptr;
            }
        }

        static inline RtMIMetadata *allocation(unsigned int numProperties) {
            if (numProperties == 0) {
                return nullptr;
            }

            auto *data            = new RtMIMetadata;
            data->m_numProperties = numProperties;
            data->m_keys          = new RetoMIString[data->m_numProperties]();
            data->m_values        = new RtMIMetadataPair[data->m_numProperties]();

            return data;
        }

        static inline void de_allocation(RtMIMetadata *data) {
            delete data;
        }

        template <typename Tp> inline void add(const std::string &key, const Tp &value) {
            auto *new_keys   = new RetoMIString[m_numProperties + 1];
            auto *new_values = new RtMIMetadataPair[m_numProperties + 1];

            for (unsigned int i = 0; i < m_numProperties; ++i) {
                new_keys[i]   = m_keys[i];
                new_values[i] = m_values[i];
            }

            delete[] m_keys;
            delete[] m_values;

            m_keys   = new_keys;
            m_values = new_values;
            m_numProperties++;

            set(m_numProperties - 1, key, value);
        }

        template <typename Tp>
        inline bool set(unsigned index, const std::string &key, const Tp &value) {
            if (index >= m_numProperties) {
                return false;
            }

            if (key.empty()) {
                return false;
            }

            m_keys[index] = key;
            //            m_values[index].m_type = GetRtMetadataType(value);
            // TODO: This line of code may lead to wrong result.
            //  Me from the future (or someone from the contributor team) should pay more attention
            //  on this.
            m_values[index].m_type = GetRtMIMetadataType<Tp>(value);

            if (nullptr != m_values[index].m_data && Rt_RTMIMetadata != m_values[index].m_type) {
                ::memcpy(m_values[index].m_data, &value, sizeof(Tp));
            } else if (nullptr != m_values[index].m_data &&
                       Rt_RTMIMetadata == m_values[index].m_type) {
                *static_cast<Tp *>(m_values[index].m_data) = value;
            } else {
                if (nullptr != m_values[index].m_data) {
                    delete static_cast<Tp *>(m_values[index].m_data);
                    m_values[index].m_data = nullptr;
                }
                m_values[index].m_data = new Tp(value);
            }

            return true;
        }

        template <typename Tp> inline bool set(const std::string &key, const Tp &value) {
            if (key.empty()) {
                return false;
            }

            bool result = false;
            for (unsigned int i = 0; i < m_numProperties; ++i) {
                if (key == m_keys[i].c_str()) {
                    set(i, key, value);
                    result = true;
                    break;
                }
            }

            return result;
        }

        template <typename Tp> inline bool get(unsigned index, Tp &value) const {
            if (index >= m_numProperties) {
                return false;
            }

            if (GetRtMetadataType(value) != m_values[index].m_type) {
                return false;
            }

            value = *static_cast<Tp *>(m_values[index].m_data);

            return true;
        }

        template <typename Tp> inline bool get(const RetoMIString &key, Tp &value) const {
            for (unsigned int i = 0; i < m_numProperties; ++i) {
                if (m_keys[i] == key) {
                    return get(i, value);
                }
            }
            return false;
        }

        template <typename Tp> inline bool get(const std::string &key, Tp &value) const {
            return get(RetoMIString(key), value);
        }

        inline bool get(size_t index, const RetoMIString *&key,
                        const RtMIMetadataPair *&entry) const {
            if (index >= m_numProperties) {
                return false;
            }

            key   = &m_keys[index];
            entry = &m_values[index];

            return true;
        }

        inline bool hasKey(const char *key) const {
            if (nullptr == key) {
                return false;
            }

            for (unsigned int i = 0; i < m_numProperties; ++i) {
                if (0 == strncmp(m_keys[i].c_str(), key, m_keys[i].length())) {
                    return true;
                }
            }
            return false;
        }

        friend bool compareKeys(const RtMIMetadata &lhs, const RtMIMetadata &rhs) {
            if (lhs.m_numProperties != rhs.m_numProperties) {
                return false;
            }

            for (unsigned int i = 0; i < lhs.m_numProperties; ++i) {
                if (lhs.m_keys[i] != rhs.m_keys[i]) {
                    return false;
                }
            }
            return true;
        }

        friend bool compareValues(const RtMIMetadata &lhs, const RtMIMetadata &rhs) {
            if (lhs.m_numProperties != rhs.m_numProperties) {
                return false;
            }

            for (unsigned int i = 0; i < lhs.m_numProperties; ++i) {
                if (lhs.m_values[i].m_type != rhs.m_values[i].m_type) {
                    return false;
                }

                switch (lhs.m_values[i].m_type) {
                case k_RtMIMetadataType::Rt_Bool: {
                    if (*static_cast<bool *>(lhs.m_values[i].m_data) !=
                        *static_cast<bool *>(rhs.m_values[i].m_data)) {
                        return false;
                    }
                } break;
                case k_RtMIMetadataType::Rt_Int32: {
                    if (*static_cast<int32_t *>(lhs.m_values[i].m_data) !=
                        *static_cast<int32_t *>(rhs.m_values[i].m_data)) {
                        return false;
                    }
                } break;
                case k_RtMIMetadataType::Rt_UnsignedInt64: {
                    if (*static_cast<uint64_t *>(lhs.m_values[i].m_data) !=
                        *static_cast<uint64_t *>(rhs.m_values[i].m_data)) {
                        return false;
                    }
                } break;
                case k_RtMIMetadataType::Rt_Float: {
                    if (*static_cast<float *>(lhs.m_values[i].m_data) !=
                        *static_cast<float *>(rhs.m_values[i].m_data)) {
                        return false;
                    }
                } break;
                case k_RtMIMetadataType::Rt_Double: {
                    if (*static_cast<double *>(lhs.m_values[i].m_data) !=
                        *static_cast<double *>(rhs.m_values[i].m_data)) {
                        return false;
                    }
                } break;
                case k_RtMIMetadataType::Rt_RTMIString: {
                    if (*static_cast<RetoMIString *>(lhs.m_values[i].m_data) !=
                        *static_cast<RetoMIString *>(rhs.m_values[i].m_data)) {
                        return false;
                    }
                } break;
                case k_RtMIMetadataType::Rt_RTMIVector3D: {
                    if (*static_cast<RtMIVector3D_Template<float> *>(lhs.m_values[i].m_data) !=
                        *static_cast<RtMIVector3D_Template<float> *>(rhs.m_values[i].m_data)) {
                        return false;
                    }
                } break;
                case k_RtMIMetadataType::Rt_RTMIMetadata: {
                    RtMIMetadata lhsNested = *static_cast<RtMIMetadata *>(lhs.m_values[i].m_data);
                    RtMIMetadata rhsNested = *static_cast<RtMIMetadata *>(rhs.m_values[i].m_data);

                    if (!compareKeys(lhsNested, rhsNested) ||
                        !compareValues(lhsNested, rhsNested)) {
                        return false;
                    }
                } break;
                case k_RtMIMetadataType::Rt_Int64: {
                    if (*static_cast<int64_t *>(lhs.m_values[i].m_data) !=
                        *static_cast<int64_t *>(rhs.m_values[i].m_data)) {
                        return false;
                    }
                } break;
                case k_RtMIMetadataType::Rt_UnsignedInt32: {
                    if (*static_cast<uint32_t *>(lhs.m_values[i].m_data) !=
                        *static_cast<uint32_t *>(rhs.m_values[i].m_data)) {
                        return false;
                    }
                } break;
                default:
                    break;
                }
            }

            return true;
        }

        friend bool operator==(const RtMIMetadata &lhs, const RtMIMetadata &rhs) {
            return compareKeys(lhs, rhs) && compareValues(lhs, rhs);
        }

        friend bool operator!=(const RtMIMetadata &lhs, const RtMIMetadata &rhs) {
            return !(lhs == rhs);
        }
    };
}; // namespace Retoccilus::Engine3D::ModelImporter