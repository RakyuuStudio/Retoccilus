#pragma once

#include <memory>
#include <string>

namespace Retoccilus::Core::FileOperation {
    class AbstractFileOperation {
    public:
        AbstractFileOperation()          = default;
        virtual ~AbstractFileOperation() = default;

        enum class FileType {
            FT_Text,
            FT_Binary,
        };

        enum class FilePermission { FP_ReadOnly, FP_WriteOnly, FP_AllPermission };

        // template <typename T>
        // virtual T readFile(const std::string &path, FileType ft = FileType::FT_Text,
        //                       FilePermission fp = FilePermission::FP_ReadOnly)   = 0;
        // virtual void writeFile(const std::string &path, FileType ft = FileType::FT_Text,
        //                        FilePermission fp = FilePermission::FP_WriteOnly) = 0;

        const std::string &data() const {
            return *m_data;
        }

    protected:
        std::unique_ptr<std::string> m_data;
    };
} // namespace Retoccilus::Core::FileOperation