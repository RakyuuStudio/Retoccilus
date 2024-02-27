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

        friend hash_code hash_value(FileEntryReference Ref) {
            return hash_value(&Ref.getFileEntry());
        }

        struct MapValue;
        using MapEntry = StringMapEntry <ErrorOr<MapValue>>;

        struct MapValue {
            PointerUnion<FileEntry *, const MapEntry *> V;
            DirectoryEntryRef Dir;

            MapValue() = delete;

            MapValue(FileEntry &FE, DirectoryEntryRef Dir) : V(&FE), Dir(Dir) {}

            MapValue(MapEntry &ME, DirectoryEntryRef Dir) : V(&ME), Dir(Dir) {}
        }

        bool isSameRef(const FileEntryReference &rhs) const { return ME == rhs.ME; }

        operator const FileEntry *() const { return &getFileEntry(); }

        FileEntryReference() = delete;

        explicit FileEntryReference(const MapEntry &ME) : ME(&ME) {
            assert(ME.second && "Expected payload");
            assert(ME.second->V && "Expected non-null");
        }

        const clang::FileEntryReference::MapEntry &getMapEntry() const { return *ME; }

        const MapEntry &getBaseMapEntry() const {
            const MapEntry *Base = ME;
            while (const auto *Next = Base->second->V.dyn_cast<const MapEntry *>())
                Base = Next;
            return *Base;
        }

    private:
        friend class FileMgr::MapEntryOptionalStorage<FileEntryReference>;

        struct optional_none_tag {
        };

        FileEntryReference(optional_none_tag): ME(nullptr) {}

        bool hasOptionalValue() const { return ME; }

        friend struct DenseMapInfo<FileEntryReference>;
        struct dense_map_empty_tag {
        };
        struct dense_map_tombstone_tag {
        };

        FileEntryReference(dense_map_empty_tag)
                : ME(DenseMapInfo<const MapEntry *>::getEmptyKey()) {}
        FileEntryReference(dense_map_tombstone_tag)
                : ME(DenseMapInfo<const MapEntry *>::getTombstoneKey()) {}

        bool isSpecialDenseMapKey() const {
            return isSameRef(FileEntryReference(dense_map_empty_tag())) ||
                   isSameRef(FileEntryReference(dense_map_tombstone_tag()));
        }

        const MapEntry *ME;
    };

    static_assert(sizeof(FileEntryReference) == sizeof(const FileEntry *),
                  "FileEntryReference must avoid size overhead");

    static_assert(is_trivially_copyable<FileEntryReference>::value,
                  "FileEntryReference must be trivially copyable");

    using OptionalFileEntryReference = CustomizableOptional<FileEntryReference>;

    namespace optionalDetailReference {
        template<>
        class OptionalStorage<clang::FileEntryReference>
                : public clang::FileMgr::MapEntryOptionalStorage<clang::FileEntryReference> {
            using StorageImpl =
                    clang::FileMgr::MapEntryOptionalStorage<clang::FileEntryReference>;

        public:
            OptionalStorage() = default;

            template<class... ArgTypes>
            explicit OptionalStorage(in_place_t, ArgTypes &&...Args)
                    : StorageImpl(in_place_t{}, forward<ArgTypes>(Args)...) {}

            OptionalStorage &operator=(clang::FileEntryReference Ref) {
                StorageImpl::operator=(Ref);
                return *this;
            }
        };

        static_assert(sizeof(OptionalFileEntryReference) == sizeof(FileEntryReference),
                      "OptionalFileEntryReference must avoid size overhead");

        static_assert(is_trivially_copyable<OptionalFileEntryReference>::value,
                      "OptionalFileEntryReference should be trivially copyable");

    } //namespace end optionalDetailReference
} //namespace end RetoCompiler

namespace retccUtils {
    template<>
    struct DenseMapInfo<clang::FileEntryReference> {
        static inline clang::FileEntryReference getEmptyKey() {
            return clang::FileEntryReference(clang::FileEntryReference::dense_map_empty_tag());
        }

        static inline clang::FileEntryReference getTombstoneKey() {
            return clang::FileEntryReference(clang::FileEntryReference::dense_map_tombstone_tag());
        }

        static unsigned getHashValue(clang::FileEntryReference Val) {
            return hash_value(Val);
        }

        static bool isEqual(clang::FileEntryReference lhs, clang::FileEntryReference rhs) {
            if (lhs.isSameRef(rhs))
                return true;

            if (lhs.isSpecialDenseMapKey() || rhs.isSpecialDenseMapKey())
                return false;

            return lhs == rhs;
        }

        static unsigned getHashValue(const clang::FileEntry *Val) {
            return hash_value(Val);
        }

        static bool isEqual(const clang::FileEntry *lhs, clang::FileEntryReference rhs) {
            if (rhs.isSpecialDenseMapKey())
                return false;
            return lhs == rhs;
        }
    };

    static unsigned getHashValue(const clang::FileEntry *Val) {
        return hash_value(Val);
    }

    static bool isEqual(const clang::FileEntry *lhs, clang::FileEntryReference rhs) {
        if (rhs.isSpecialDenseMapKey())
            return false;
        return lhs == rhs;
    }

} //namespace end retccUtils

namespace RetoCompiler {
    inline bool operator==(const FileEntry *lhs, const OptionalFileEntryReference &rhs) {
        return lhs == (rhs ? &rhs->getFileEntry() : nullptr);
    }

    inline bool operator==(const OptionalFileEntryReference &lhs, const FileEntry *rhs) {
        return (lhs ? &lhs->getFileEntry() : nullptr) == rhs;
    }

    inline bool operator!=(const FileEntry *lhs, const OptionalFileEntryReference &rhs) {
        return !(lhs == rhs);
    }

    inline bool operator!=(const OptionalFileEntryReference &lhs, const FileEntry *rhs) {
        return !(lhs == rhs);
    }

    class FileEntry {
        friend class FileManager;

        friend class FileEntryTestHelper;

        FileEntry();

        FileEntry(const FileEntry &) = delete;

        FileEntry &operator=(const FileEntry &) = delete;

        string RealPathName;
        off_t Size = 0;
        time_t ModTime = 0;
        const DirectoryEntry *Dir = nullptr;

        sys::fs::UniqueID UniqueID;
        unsigned UID = 0; // A unique (small) ID for the file.
        bool IsNamedPipe = false;

        mutable unique_ptr<virtuslFileSystem::File> File;

        unique_ptr<MemoryBuffer> Content;

        OptionalFileEntryReference LastRef;
    public:
        ~FileEntry();

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

    off_t FileEntryReference::getSize() const { return getFileEntry().getSize(); }

    unsigned FileEntryReference::getUID() const { return getFileEntry().getUID(); }

    const sys::fs::UniqueID &FileEntryReference::getUniqueID() const {
        return getFileEntry().getUniqueID();
    }

    time_t FileEntryReference::getModificationTime() const {
        return getFileEntry().getModificationTime();
    }

    bool FileEntryReference::isNamedPipe() const { return getFileEntry().isNamedPipe(); }

    void FileEntryReference::closeFile() const { getFileEntry().closeFile(); }
} //namespace end RetoCompiler

#endif //Rtcc_PreprocessorLexer_h