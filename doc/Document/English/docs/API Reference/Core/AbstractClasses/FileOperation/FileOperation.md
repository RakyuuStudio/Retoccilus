# `FileOperation` Reference

This class supports the File I/O Operation.

|           |                                                                    |
| --------- | ------------------------------------------------------------------ |
| **Header** | `#include <Core/AbstractClasses/FileOperation/FileOperation.hpp>` |
| **Inherits From** | `AbstractFileOperation`                                         |

### Public Functions & Members

|   |   |
| ---: | - |
| *[concept](https://en.cppreference.com/w/cpp/concepts)*  | FileReadResult |

#### `FileOperation` Class
|   |   |
| ---: | - |
| Default Constructor | `FileOperation() = default`|
| Default Destructor  | `~FileOperation() = default` |
| [std::future](https://en.cppreference.com/w/cpp/thread/future) `<T>`      | `readFileAsync(const std::string &path, FileType ft = FileType::FT_Text, FilePermission fp = FilePermission::FP_ReadOnly)` |
| T                   | `readFile(const std::string &path, FileType ft = FileType::FT_Text, FilePermission fp = FilePermission::FP_ReadOnly)` | 
| [std::future](https://en.cppreference.com/w/cpp/thread/future) `<void>` | `writeFileAsync(const std::string &path, const std::vector<char> &data, FileType ft       = FileType::FT_Binary, FilePermission fp = FilePermission::FP_WriteOnly)` |
| void                 | `writeFile(const std::string &path, const std::vector<char> &data, FileType ft       = FileType::FT_Binary, FilePermission fp = FilePermission::FP_WriteOnly)` |

#### `MemoryPool` Class
|   |   |
| ---: | - |
| *static constexpr* [size_t](https://en.cppreference.com/w/cpp/types/size_t) | `BLOCK_SIZE` |
| [std::vector<char>](https://en.cppreference.com/w/cpp/container/vector) | `&acquireBuffer()` |
| void | releaseBuffer(std::vector<char> &buffer) |

#### `FileHandle` Class
> Note: Class `FileHandle` is a class that the content is determinated by different platform. So I will write it in two sheets to mark different platform.

### Private Functions & Members
#### `FileOperation` Class

> `NativeHandle` is a type alias, the original type of this alias is determinated by platform. On Windows, it's `HANDLE` type from `<windows.h>`, on Linux or macOS or other systems based on POSIX, it's `int` type.

|    | |
| -: | - |
| *Type Alias* NativeHandle | `HANDLE` (Windows); `int` (POSIX) |
| FileHandle | `openFile(const std::string &path, FilePermission fp)` |
| T | `readDataBuffered(NativeHandle handle, size_t fileSize)` |
| void | `writeDataBuffered(NativeHandle handle, const std::vector<char> &data)` |
| [size_t](https://en.cppreference.com/w/cpp/types/size_t) | `getFileSize(const std::string &path)` |
| [size_t](https://en.cppreference.com/w/cpp/types/size_t) | `readFileInternal(NativeHandle handle, void *buffer, size_t bufferSize)` |
| [size_t](https://en.cppreference.com/w/cpp/types/size_t) | `writeFileInternal(NativeHandle handle, const void *buffer, size_t bufferSize)` |
| [std::string](https://en.cppreference.com/w/cpp/string/basic_string) | `getLastError()` |
| *static* MemoryPool | m_memoryPool |

#### `MemoryPool` Class
|    | |
| -: | - |
| [std::mutex](https://en.cppreference.com/w/cpp/thread/mutex) | `m_mutex` |
| [std::deque](https://en.cppreference.com/w/cpp/container/deque) | `m_pool` |
| [std::list](https://en.cppreference.com/w/cpp/container/list) | `m_inUse` |