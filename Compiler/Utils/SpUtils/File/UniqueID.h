#ifndef Rtcc_UniqueID_h
#define Rtcc_UniqueID_h

#include "Utils/AbstractDataType/DenseMapInfo.h"
#include "Utils/AbstractDataType/Hashing.h"

#include <cstdint>
#include <utility>

namespace retccUtils {
    namespace System {
        namespace fileS {

            class UniqueID {
                uint64_t Device;
                uint64_t File;

            public:
                UniqueID() = default;

                UniqueID(uint64_t Device, uint64_t File) : Device(Device), File(File) {}

                bool operator==(const UniqueID &Other) const {
                    return Device == Other.Device && File == Other.File;
                }

                bool operator!=(const UniqueID &Other) const { return !(*this == Other); }

                bool operator<(const UniqueID &Other) const {
                    if (Device < Other.Device)
                        return true;
                    if (Other.Device < Device)
                        return false;
                    return File < Other.File;
                }

                uint64_t getDevice() const { return Device; }
                uint64_t getFile() const { return File; }
            };

        } // //namespace end fileS
    } //namespace end System

    template<>
    struct DenseMapInfo<retccUtils::System::fileS::UniqueID> {
        static inline retccUtils::System::fileS::UniqueID getEmptyKey() {
            auto EmptyKey = DenseMapInfo<std::pair<uint64_t, uint64_t>>::getEmptyKey();
            return {EmptyKey.first, EmptyKey.second};
        }

        static inline retccUtils::System::fileS::UniqueID getTombstoneKey() {
            auto TombstoneKey =
                    DenseMapInfo<std::pair<uint64_t, uint64_t>>::getTombstoneKey();
            return {TombstoneKey.first, TombstoneKey.second};
        }

        static hash_code getHashValue(const retccUtils::System::fileS::UniqueID &Tag) {
            return hash_value(std::make_pair(Tag.getDevice(), Tag.getFile()));
        }

        static bool isEqual(const retccUtils::System::fileS::UniqueID &lhs,
                            const retccUtils::System::fileS::UniqueID &rhs) {
            return lhs == rhs;
        }
    };

} //namespace end retccUtils

#endif