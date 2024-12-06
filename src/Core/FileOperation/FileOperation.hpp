/**
 * @file FileOperation.hpp
 * @author Almond Ryan (ryan_nvkopres@outlook.com)
 * @brief This file contains the source of multi-platform file operation.

 * @version 1.0
 * @date 2024-12-03
 *
 * @copyright Copyright (c) 2023-2025 Rakyuu Studio and All Contributors. All rights reserved.
 * @copyright This file is a part of Retoccilus, which licensed under Apache 2.0 with Retoccilus
 Exception.
 */

#pragma once

#include "AbstractFileOperation.hpp"
#include <cstddef>
#include <future>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>
#include <list>
#include <cstring>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#undef min
#undef max
#else
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace Retoccilus::Core::FileOperation {
    /**
     * @class MemoryPool
     *
     * @brief This class declares a Memory Pool Object which can be useful in re-using the existing
     * instances.
     */
    class MemoryPool {
    public:
        static constexpr size_t BLOCK_SIZE = 8192;

        /**
         * @brief Acquire a buffer object to use.
         * This function try to get a available character vector as a buffer from the internal pool.
         * If the pool is empty, we create a new buffer the initialize the size to BLOCK_SIZE (which
         * is 8192).
         *
         * The buffer we get will be move into a list that we are using, then returns the reference.
         * We promise the operation is thread-safe, because it will protected by `m_mutex` mutex
         * lock.
         *
         * @warning The reference will be unavailable when we called `releaseBuffer()` or other
         * thread release the same buffer.
         *
         * @return std::vector<char>& - A reference of character vector used to the read and write
         * operation.
         */
        std::vector<char> &acquireBuffer() {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_pool.empty()) {
                m_pool.emplace_back(BLOCK_SIZE);
            }
            auto buffer = std::move(m_pool.front());
            m_pool.pop_front();
            return m_inUse.emplace_back(std::move(buffer));
        }

        /**
         * @brief Release the buffer we get previously to the pool.
         *
         * If we don't require to use the buffer that `acquireBuffer()` returns, please call this
         * function to return the buffer to pool.
         *
         * This function will clear the content we provide, and move it back to pool, to
         * re-allocate. And obviously, this function is also thread-safe.
         *
         * @param buffer - A reference that get from `acquireBuffer()` function.
         *
         * @warning You MUST make sure the buffer that provided to this function is allocate from
         * `acquireBuffer()`, or it will cause undefined behavior or resource(memory) leak.
         */
        void releaseBuffer(std::vector<char> &buffer) {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = std::find_if(m_inUse.begin(), m_inUse.end(),
                                   [&buffer](const auto &b) { return &b == &buffer; });
            if (it != m_inUse.end()) {
                buffer.clear();
                m_pool.push_back(std::move(*it));
                m_inUse.erase(it);
            }
        }

    private:
        std::mutex m_mutex;
        std::deque<std::vector<char>> m_pool;
        std::list<std::vector<char>> m_inUse;
    };

    template <typename T>
    concept FileReadResult = std::is_same_v<T, std::string> || std::is_same_v<T, std::vector<char>>;

    /**
     * @class FileHandle
     *
     * @brief This class declares a File Handle object to operate the file.
     */
    class FileHandle {
    public:
#ifdef _WIN32
        /**
         * @brief Construct a new File Handle object, to receive the `HANDLE` type parameter from
         * Windows.
         * @param handle - The valid handle from Windows.
         * @note The body of Constructor can be empty, because the initialization steps is done by
         * the initialize list.
         */
        explicit FileHandle(HANDLE handle) : m_handle(handle) {
        }

        /**
         * @brief Destroy the File Handle object
         */
        ~FileHandle() {
            if (m_handle != INVALID_HANDLE_VALUE) {
                CloseHandle(m_handle);
            }
        }

        /**
         * @brief Get the handle to the internally stored Windows file.
         * @return HANDLE - Returns the Windows file handle associated with the current instance.
         */
        HANDLE get() const {
            return m_handle;
        }

    private:
        /**
         * @brief Stores the file handle for the Windows platform, the default value is an invalid
         * handle.
         */
        HANDLE m_handle = INVALID_HANDLE_VALUE;
#else
        /**
         * @brief Construct a new File Handle object, receives an integer-type file descriptor for
         * POSIX-compatible platforms.

         * @param fd - A valid file descriptor returned by a POSIX system call.
         * @note The body of Constructor can be empty, because the initialization steps is done by
         the initialize list.
         */
        explicit FileHandle(int fd) : m_fd(fd) {
        }

        /**
         * @brief Destroy the File Handle object, responsible for closing file descriptors for
         * POSIX-compatible platforms. If the file descriptor is valid, close it with the close
         * function.
         */
        ~FileHandle() {
            if (m_fd != -1) {
                close(m_fd);
            }
        }

        /**
         * @brief Get the POSIX file descriptor for internal storage.
         * @return int - Returns the POSIX file descriptor associated with the current instance.
         */
        int get() const {
            return m_fd;
        }

    private:
        int m_fd = -1;
#endif
    };

    /**
     * @class MemoryMappedFile
     *
     * @brief This class declares a Memory Mapped File object to operate the file.
     */
    class MemoryMappedFile {
    public:
        /**
         * @brief Construct a new Memory Mapped File object, receives a file path and a size of the
         * file.
         * @param path - The path of the file to be mapped.
         * @param size - The size of the file to be mapped.
         * @param write - A boolean value indicating whether the file should be opened for writing.
         * @note The body of Constructor can be empty, because the initialization steps is done by
         * the initialize list.
         */
        MemoryMappedFile(const std::string &path, size_t size, bool write = false) {
#ifdef _WIN32
            m_fileHandle = CreateFileA(
                path.c_str(), write ? GENERIC_READ | GENERIC_WRITE : GENERIC_READ, FILE_SHARE_READ,
                nullptr, write ? CREATE_ALWAYS : OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

            if (m_fileHandle == INVALID_HANDLE_VALUE) {
                throw std::runtime_error("Failed to open file for mapping");
            }

            m_mappingHandle = CreateFileMappingA(
                m_fileHandle, nullptr, write ? PAGE_READWRITE : PAGE_READONLY, 0, size, nullptr);

            if (!m_mappingHandle) {
                CloseHandle(m_fileHandle);
                throw std::runtime_error("Failed to create file mapping");
            }

            m_data =
                MapViewOfFile(m_mappingHandle, write ? FILE_MAP_WRITE : FILE_MAP_READ, 0, 0, size);

            if (!m_data) {
                CloseHandle(m_mappingHandle);
                CloseHandle(m_fileHandle);
                throw std::runtime_error("Failed to map view of file");
            }
#else
            m_fd = open(path.c_str(), write ? (O_RDWR | O_CREAT) : O_RDONLY, 0644);
            if (m_fd == -1) {
                throw std::runtime_error("Failed to open file for mapping");
            }

            if (write) {
                if (ftruncate(m_fd, size) == -1) {
                    close(m_fd);
                    throw std::runtime_error("Failed to set file size");
                }
            }

            m_data = mmap(nullptr, size, write ? (PROT_READ | PROT_WRITE) : PROT_READ, MAP_SHARED,
                          m_fd, 0);

            if (m_data == MAP_FAILED) {
                close(m_fd);
                throw std::runtime_error("Failed to map file");
            }
#endif
            m_size = size;
        }

        /**
         * @brief Destroy the Memory Mapped File object, responsible for unmapping and closing the
         * file mapping.
         */
        ~MemoryMappedFile() {
#ifdef _WIN32
            if (m_data)
                UnmapViewOfFile(m_data);
            if (m_mappingHandle)
                CloseHandle(m_mappingHandle);
            if (m_fileHandle != INVALID_HANDLE_VALUE)
                CloseHandle(m_fileHandle);
#else
            if (m_data != MAP_FAILED)
                munmap(m_data, m_size);
            if (m_fd != -1)
                close(m_fd);
#endif
        }

        void *data() {
            return m_data;
        }
        const void *data() const {
            return m_data;
        }
        size_t size() const {
            return m_size;
        }

    private:
#ifdef _WIN32
        HANDLE m_fileHandle    = INVALID_HANDLE_VALUE;
        HANDLE m_mappingHandle = nullptr;
#else
        int m_fd = -1;
#endif
        void *m_data  = nullptr;
        size_t m_size = 0;
    };

    /**
     * @class FileOperation
     * @brief This class declares a File Operation object to operate the file.
     */
    class FileOperation : public AbstractFileOperation {
    public:
        /**
         * @brief Construct a new File Operation object
         */
        FileOperation() = default;

        /**
         * @brief Destroy the File Operation object
         */
        ~FileOperation() override = default;

        /**
         * @brief Read the file from the given path and return the result.
         * @tparam T - The type of the result to be returned.
         * @param path - The path of the file to be read.
         * @param ft - The type of the file to be read.
         * @param fp - The permission of the file to be read.

         * @return T - The result of the file read operation.
         */
        template <FileReadResult T>
        std::future<T> readFileAsync(const std::string &path, FileType ft = FileType::FT_Text,
                                     FilePermission fp = FilePermission::FP_ReadOnly) {
            return std::async(std::launch::async,
                              [this, path, ft, fp]() { return readFile<T>(path, ft, fp); });
        }

        /**
         * @brief Read the file from the given path and return the result.

         * @tparam T - The type of the result to be returned.
         * @param path - The path of the file to be read.
         * @param ft - The type of the file to be read.
         * @param fp - The permission of the file to be read.
         * @return T - The result of the file read operation.
         */
        template <FileReadResult T>
        T readFile(const std::string &path, FileType ft = FileType::FT_Text,
                   FilePermission fp = FilePermission::FP_ReadOnly) {
            size_t fileSize = getFileSize(path);

            if (fileSize < 1024 * 1024) {
                MemoryMappedFile mmf(path, fileSize);
                if constexpr (std::is_same_v<T, std::string>) {
                    return std::string(static_cast<const char *>(mmf.data()), fileSize);
                } else {
                    return std::vector<char>(static_cast<const char *>(mmf.data()),
                                             static_cast<const char *>(mmf.data()) + fileSize);
                }
            }

            auto fileHandle = openFile(path, fp);
            return readDataBuffered<T>(fileHandle.get(), fileSize);
        }

        /**
         * @brief Write the file to the given path.

         * @param path - The path of the file to be written.
         * @param data - The data to be written to the file.
         * @param ft - The type of the file to be written.
         * @param fp - The permission of the file to be written.
         */
        std::future<void> writeFileAsync(const std::string &path, const std::vector<char> &data,
                                         FileType ft       = FileType::FT_Binary,
                                         FilePermission fp = FilePermission::FP_WriteOnly) {
            return std::async(std::launch::async,
                              [this, path, data, ft, fp]() { writeFile(path, data, ft, fp); });
        }

        /**
         * @brief Write the file to the given path.

         * @param path - The path of the file to be written.
         * @param data - The data to be written to the file.
         * @param ft - The type of the file to be written.
         * @param fp - The permission of the file to be written.
         */
        void writeFile(const std::string &path, const std::vector<char> &data,
                       FileType ft       = FileType::FT_Binary,
                       FilePermission fp = FilePermission::FP_WriteOnly) {
            if (data.size() < 1024 * 1024) {
                MemoryMappedFile mmf(path, data.size(), true);
                std::memcpy(mmf.data(), data.data(), data.size());
                return;
            }

            auto fileHandle = openFile(path, fp);
            writeDataBuffered(fileHandle.get(), data);
        }

    private:
#ifdef _WIN32
        using NativeHandle = HANDLE;
#else
        using NativeHandle = int;
#endif

        /**
         * @brief Open the file with the given path and permission.

         * @param path - The path of the file to be opened.
         * @param fp - The permission of the file to be opened.
         * @return FileHandle - The handle of the opened file.
         */
        FileHandle openFile(const std::string &path, FilePermission fp) {
#ifdef _WIN32
            DWORD desiredAccess       = 0;
            DWORD creationDisposition = OPEN_EXISTING;

            switch (fp) {
            case FilePermission::FP_ReadOnly:
                desiredAccess = GENERIC_READ;
                break;
            case FilePermission::FP_WriteOnly:
                desiredAccess       = GENERIC_WRITE;
                creationDisposition = CREATE_ALWAYS;
                break;
            case FilePermission::FP_AllPermission:
                desiredAccess       = GENERIC_READ | GENERIC_WRITE;
                creationDisposition = CREATE_ALWAYS;
                break;
            default:
                throw std::invalid_argument("Invalid file permission");
            }

            HANDLE handle = CreateFileA(path.c_str(), desiredAccess, FILE_SHARE_READ, nullptr,
                                        creationDisposition, FILE_ATTRIBUTE_NORMAL, nullptr);

            if (handle == INVALID_HANDLE_VALUE) {
                throw std::runtime_error("Failed to open file: " + getLastError());
            }
            return FileHandle(handle);
#else
            int flags   = 0;
            mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

            switch (fp) {
            case FilePermission::FP_ReadOnly:
                flags = O_RDONLY;
                break;
            case FilePermission::FP_WriteOnly:
                flags = O_WRONLY | O_CREAT | O_TRUNC;
                break;
            case FilePermission::FP_AllPermission:
                flags = O_RDWR | O_CREAT | O_TRUNC;
                break;
            default:
                throw std::invalid_argument("Invalid file permission");
            }

            int fd = open(path.c_str(), flags, mode);
            if (fd == -1) {
                throw std::runtime_error("Failed to open file: " + std::string(strerror(errno)));
            }
            return FileHandle(fd);
#endif
        }

        /**
         * @brief Read the data from the given file handle.

         * @param handle - The handle of the file to be read.
         * @param fileSize - The size of the file to be read.
         * @return T - The result of the file read operation.
         */
        template <FileReadResult T> T readDataBuffered(NativeHandle handle, size_t fileSize) {
            auto &buffer = m_memoryPool.acquireBuffer();

            if constexpr (std::is_same_v<T, std::string>) {
                std::string result;
                result.reserve(fileSize);

                size_t totalRead = 0;
                while (totalRead < fileSize) {
                    size_t toRead    = std::min(buffer.size(), fileSize - totalRead);
                    size_t bytesRead = readFileInternal(handle, buffer.data(), toRead);
                    if (bytesRead == 0)
                        break;

                    result.append(buffer.data(), bytesRead);
                    totalRead += bytesRead;
                }

                m_memoryPool.releaseBuffer(buffer);
                return result;
            } else {
                std::vector<char> result;
                result.reserve(fileSize);

                size_t totalRead = 0;
                while (totalRead < fileSize) {
                    size_t toRead    = std::min(buffer.size(), fileSize - totalRead);
                    size_t bytesRead = readFileInternal(handle, buffer.data(), toRead);
                    if (bytesRead == 0)
                        break;

                    result.insert(result.end(), buffer.data(), buffer.data() + bytesRead);
                    totalRead += bytesRead;
                }

                m_memoryPool.releaseBuffer(buffer);
                return result;
            }
        }

        /**
         * @brief Write the data to the given file handle.

         * @param handle - The handle of the file to be written.
         * @param data - The data to be written to the file.
         */
        void writeDataBuffered(NativeHandle handle, const std::vector<char> &data) {
            auto &buffer = m_memoryPool.acquireBuffer();

            size_t totalWritten = 0;
            while (totalWritten < data.size()) {
                size_t toWrite = std::min(buffer.size(), data.size() - totalWritten);
                std::memcpy(buffer.data(), data.data() + totalWritten, toWrite);

                size_t bytesWritten = writeFileInternal(handle, buffer.data(), toWrite);
                if (bytesWritten == static_cast<size_t>(-1)) {
                    m_memoryPool.releaseBuffer(buffer);
                    throw std::runtime_error("Failed to write file");
                }
                totalWritten += bytesWritten;
            }

            m_memoryPool.releaseBuffer(buffer);
        }

        /**
         * @brief Get the size of the file.

         * @param path - The path of the file.
         * @return size_t - The size of the file.
         */
        size_t getFileSize(const std::string &path) {
#ifdef _WIN32
            WIN32_FILE_ATTRIBUTE_DATA fad;
            if (!GetFileAttributesExA(path.c_str(), GetFileExInfoStandard, &fad)) {
                throw std::runtime_error("Failed to get file size");
            }
            return (static_cast<size_t>(fad.nFileSizeHigh) << 32) | fad.nFileSizeLow;
#else
            struct stat st;
            if (stat(path.c_str(), &st) == -1) {
                throw std::runtime_error("Failed to get file size");
            }
            return static_cast<size_t>(st.st_size);
#endif
        }

        /**
         * @brief Read data from the given file handle.

         * @param handle - The handle of the file to be read.
         * @param buffer - The buffer to store the read data.
         * @param bufferSize - The size of the buffer.
         * @return size_t - The number of bytes read.
         */
        size_t readFileInternal(NativeHandle handle, void *buffer, size_t bufferSize) {
#ifdef _WIN32
            DWORD bytesRead;
            if (!ReadFile(handle, buffer, static_cast<DWORD>(bufferSize), &bytesRead, nullptr)) {
                throw std::runtime_error("ReadFile failed: " + getLastError());
            }
            return bytesRead;
#else
            ssize_t result = read(handle, buffer, bufferSize);
            if (result < 0) {
                throw std::runtime_error("read failed: " + std::string(strerror(errno)));
            }
            return static_cast<size_t>(result);
#endif
        }

        /**
         * @brief Write data to the given file handle.

         * @param handle - The handle of the file to be written.
         * @param buffer - The buffer containing the data to be written.
         * @param bufferSize - The size of the buffer.
         * @return size_t - The number of bytes written.
         */
        size_t writeFileInternal(NativeHandle handle, const void *buffer, size_t bufferSize) {
#ifdef _WIN32
            DWORD bytesWritten;
            if (!WriteFile(handle, buffer, static_cast<DWORD>(bufferSize), &bytesWritten,
                           nullptr)) {
                return static_cast<size_t>(-1);
            }
            return bytesWritten;
#else
            ssize_t result = write(handle, buffer, bufferSize);
            if (result < 0) {
                return static_cast<size_t>(-1);
            }
            return static_cast<size_t>(result);
#endif
        }

        /**
         * @brief Get the last error message.

         * @return std::string - The last error message.
         */
        std::string getLastError() {
#ifdef _WIN32
            DWORD error = GetLastError();
            LPVOID msgBuf;
            FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                               FORMAT_MESSAGE_IGNORE_INSERTS,
                           nullptr, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                           (LPSTR)&msgBuf, 0, nullptr);
            std::string errorMsg(static_cast<char *>(msgBuf));
            LocalFree(msgBuf);
            return errorMsg;
#else
            return strerror(errno);
#endif
        }

    private:
        static MemoryPool m_memoryPool;
    };

    MemoryPool FileOperation::m_memoryPool;
} // namespace Retoccilus::Core::FileOperation