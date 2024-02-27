#ifndef Rtcc_PreprocessorLexer_h
#define Rtcc_PreprocessorLexer_h

#include "Utils/Basic/CustomizableOptional.h"
#include "Utils/Basic/DirectoryEntrance.h"
#include "Utils/Basic/CompilerReference.h"
#include "Utils/AbstractDataType/DenseMapInfo.h"
#include "Utils/AbstractDataType/Hashing.h"
#include "Utils/AbstractDataType/PointerUnionX.h"
#include "Utils/AbstractDataType/StringMap.h"
#include "Utils/AbstractDataType/StringReference.h"
#include "Utils/SpUtils/ErrorX.h"
#include "Utils/SpUtils/File/UniqueID.h"

#include <optional>
#include <utility>
#include <tuple>
#include <string>

using namespace std;
namespace retccUtils {
    class MemoryBuffer;
    namespace virtualFileSystem {
        class File;
    } //namespace end virtualFileSystem
} //namespace end retccUtils

namespace RetoCompiler {
    class FileEntranceReference; //Forward Declaration
    namespace stdOptionalDetail {
        template<>
        class ROptionalStorage<RetoCompiler::FileEntranceReference>;
    } //namespace end stdOptionalDetail

    class FileEntranceInstance;

    class FileEntranceReference {
    public:
        const StringReference getName() {
            return getBaseMappingMapEntrance().first();
        }

        const StringReference getNameAsUserRequested() {
            return ME->first();
        }

        const FileEntrance &getFileEntrance() {
            return *getBaseMappingMapEntrance().second->V.get<FileEntrance *>();
        }

        DirectoryEntranceRef getDir() const { return ME->second->Dir; }

        inline off_t getSize() const;

        inline unsigned getUID() const;

        inline const sys::fs::UniqueID &getUniqueID() const;

        inline time_t getModificationTime() const;

        inline bool isNamedPipe() const;

        inline void closeFile() const;

        friend bool operator==(const FileEntranceReference &lhs, const FileEntranceReference &rhs) {
            return &lhs.getFileEntrance() == &rhs.getFileEntrance();
        }

        friend bool operator==(const FileEntrance *lhs, const FileEntranceReference &rhs) {
            return lhs == &rhs.getFileEntrance();
        }

        friend bool operator==(const FileEntranceReference &lhs, const FileEntrance *rhs) {
            return &lhs.getFileEntrance() == rhs;
        }

        friend bool operator!=(const FileEntranceReference &lhs, const FileEntranceReference &rhs) {
            return !(lhs == rhs);
        }

        friend bool operator!=(const FileEntrance *lhs, const FileEntranceReference &rhs) {
            return !(lhs == rhs);
        }

        friend bool operator!=(const FileEntranceReference &lhs, const FileEntrance *rhs) {
            return !(lhs == rhs);
        }

        friend hash_code hash_value(FileEntranceReference Ref) {
            return hash_value(&Ref.getFileEntrance());
        }

        struct MapValue;
        using MapEntry = StringMapEntry <ErrorOr<MapValue>>;

        struct MapValue {
            PointerUnion<FileEntrance *, const MapEntry *> V;
            DirectoryEntryRef Dir;

            MapValue() = delete;

            MapValue(FileEntrance &FE, DirectoryEntryRef Dir) : V(&FE), Dir(Dir) {}

            MapValue(MapEntry &ME, DirectoryEntryRef Dir) : V(&ME), Dir(Dir) {}
        }

        bool isSameRef(const FileEntranceReference &rhs) const { return ME == rhs.ME; }

        operator const FileEntrance *() const { return &getFileEntrance(); }

        FileEntranceReference() = delete;

        explicit FileEntranceReference(const MapEntry &ME) : ME(&ME) {
            assert(ME.second && "Expected payload");
            assert(ME.second->V && "Expected non-null");
        }

        const clang::FileEntranceReference::MapEntry &getMapEntry() const { return *ME; }

        const MapEntry &getBaseMapEntry() const {
            const MapEntry *Base = ME;
            while (const auto *Next = Base->second->V.dyn_cast<const MapEntry *>())
                Base = Next;
            return *Base;
        }

    private:
        friend class FileMgr::MapEntryOptionalStorage<FileEntranceReference>;

        struct optional_none_tag {
        };

        FileEntranceReference(optional_none_tag): ME(nullptr) {}

        bool hasOptionalValue() const { return ME; }

        friend struct DenseMapInfo<FileEntranceReference>;
        struct dense_map_empty_tag {
        };
        struct dense_map_tombstone_tag {
        };

        FileEntranceReference(dense_map_empty_tag)
                : ME(DenseMapInfo<const MapEntry *>::getEmptyKey()) {}
        FileEntranceReference(dense_map_tombstone_tag)
                : ME(DenseMapInfo<const MapEntry *>::getTombstoneKey()) {}

        bool isSpecialDenseMapKey() const {
            return isSameRef(FileEntranceReference(dense_map_empty_tag())) ||
                   isSameRef(FileEntranceReference(dense_map_tombstone_tag()));
        }

        const MapEntry *ME;
    };

    static_assert(sizeof(FileEntranceReference) == sizeof(const FileEntrance *),
                  "FileEntranceReference must avoid size overhead");

    static_assert(is_trivially_copyable<FileEntranceReference>::value,
                  "FileEntranceReference must be trivially copyable");

    using OptionalFileEntranceReference = CustomizableOptional<FileEntranceReference>;

    namespace optionalDetailReference {
        template<>
        class OptionalStorage<clang::FileEntranceReference>
                : public clang::FileMgr::MapEntryOptionalStorage<clang::FileEntranceReference> {
            using StorageImpl =
                    clang::FileMgr::MapEntryOptionalStorage<clang::FileEntranceReference>;

        public:
            OptionalStorage() = default;

            template<class... ArgTypes>
            explicit OptionalStorage(in_place_t, ArgTypes &&...Args)
                    : StorageImpl(in_place_t{}, forward<ArgTypes>(Args)...) {}

            OptionalStorage &operator=(clang::FileEntranceReference Ref) {
                StorageImpl::operator=(Ref);
                return *this;
            }
        };

        static_assert(sizeof(OptionalFileEntranceReference) == sizeof(FileEntranceReference),
                      "OptionalFileEntranceReference must avoid size overhead");

        static_assert(is_trivially_copyable<OptionalFileEntranceReference>::value,
                      "OptionalFileEntranceReference should be trivially copyable");

    } //namespace end optionalDetailReference
} //namespace end RetoCompiler

namespace retccUtils {
    template<>
    struct DenseMapInfo<clang::FileEntranceReference> {
        static inline clang::FileEntranceReference getEmptyKey() {
            return clang::FileEntranceReference(clang::FileEntranceReference::dense_map_empty_tag());
        }

        static inline clang::FileEntranceReference getTombstoneKey() {
            return clang::FileEntranceReference(clang::FileEntranceReference::dense_map_tombstone_tag());
        }

        [[maybe_unused]] static unsigned getHashValue(clang::FileEntranceReference Val) {
            return hash_value(Val);
        }

        [[maybe_unused]] static bool isEqual(clang::FileEntranceReference lhs, clang::FileEntranceReference rhs) {
            if (lhs.isSameRef(rhs))
                return true;

            if (lhs.isSpecialDenseMapKey() || rhs.isSpecialDenseMapKey())
                return false;

            return lhs == rhs;
        }

        [[maybe_unused]] static unsigned getHashValue(const clang::FileEntrance *Val) {
            return hash_value(Val);
        }

        [[maybe_unused]] static bool isEqual(const clang::FileEntrance *lhs, clang::FileEntranceReference rhs) {
            if (rhs.isSpecialDenseMapKey())
                return false;
            return lhs == rhs;
        }
    };

    static unsigned getHashValue(const clang::FileEntrance *Val) {
        return hash_value(Val);
    }

    static bool isEqual(const clang::FileEntrance *lhs, clang::FileEntranceReference rhs) {
        if (rhs.isSpecialDenseMapKey())
            return false;
        return lhs == rhs;
    }

} //namespace end retccUtils

namespace RetoCompiler {
    inline bool operator==(const FileEntrance *lhs, const OptionalFileEntranceReference &rhs) {
        return lhs == (rhs ? &rhs->getFileEntrance() : nullptr);
    }

    inline bool operator==(const OptionalFileEntranceReference &lhs, const FileEntrance *rhs) {
        return (lhs ? &lhs->getFileEntrance() : nullptr) == rhs;
    }

    inline bool operator!=(const FileEntrance *lhs, const OptionalFileEntranceReference &rhs) {
        return !(lhs == rhs);
    }

    inline bool operator!=(const OptionalFileEntranceReference &lhs, const FileEntrance *rhs) {
        return !(lhs == rhs);
    }

    class FileEntrance {
        friend class FileManager;

        friend class FileEntranceTestHelper;

        FileEntrance();

        FileEntrance(const FileEntrance &) = delete;

        FileEntrance &operator=(const FileEntrance &) = delete;

        string RealPathName;
        off_t Size = 0;
        time_t ModTime = 0;
        const DirectoryEntry *Dir = nullptr;

        sys::fs::UniqueID UniqueID;
        unsigned UID = 0; // A unique (small) ID for the file.
        bool IsNamedPipe = false;

        mutable unique_ptr<virtuslFileSystem::File> File;

        unique_ptr<MemoryBuffer> Content;

        OptionalFileEntranceReference LastRef;
    public:
        ~FileEntrance();

        StringRef getName() const { return LastRef->getName(); }

        StringRef tryGetRealPathName() const { return RealPathName; }

        off_t getSize() const { return Size; }

        unsigned getUID() const { return UID; }

        const sys::fs::UniqueID &getUniqueID() const { return UniqueID; }

        time_t getModificationTime() const { return ModTime; }

        const DirectoryEntry *getDir() const { return Dir; }

        bool isNamedPipe() const { return IsNamedPipe; }

        void closeFile() const;
    };

    off_t FileEntranceReference::getSize() const { return getFileEntrance().getSize(); }

    unsigned FileEntranceReference::getUID() const { return getFileEntrance().getUID(); }

    const sys::fs::UniqueID &FileEntranceReference::getUniqueID() const {
        return getFileEntrance().getUniqueID();
    }

    time_t FileEntranceReference::getModificationTime() const {
        return getFileEntrance().getModificationTime();
    }

    bool FileEntranceReference::isNamedPipe() const { return getFileEntrance().isNamedPipe(); }

    void FileEntranceReference::closeFile() const { getFileEntrance().closeFile(); }
} //namespace end RetoCompiler

#endif //Rtcc_PreprocessorLexer_h