#ifndef RETO_MI_STRING_INCLUDE_H
#define RETO_MI_STRING_INCLUDE_H

#include <limits>
#include <string>

namespace Retoccilus::Engine3D::ModelImporter {
    class RetoMIString {
    public:
        // It should be '\0' when initialize
        RetoMIString() noexcept : length(0), data(nullptr) {
            data = new char[1];
            *data = '\0';
        }

        explicit RetoMIString(const std::string &str) : length(static_cast<unsigned long>(str.size())), data(nullptr) {
            data = new char[length + 1];
            std::memcpy(data, str.c_str(), length);
            data[length] = '\0';
        }

        RetoMIString(const RetoMIString &other) : length(other.length), data(nullptr) {
            if (length > 0) {
                data = new char[length + 1];
                std::memcpy(data, other.data, length + 1);
            } else {
                data = new char[1];
                *data = '\0';
            }
        }

        RetoMIString& operator=(const RetoMIString &other) {
            if (this != &other) {
                delete[] data;
                length = other.length;
                if (length > 0) {
                    data = new char[length + 1];
                    std::memcpy(data, other.data, length + 1);
                } else {
                    data = new char[1];
                    *data = '\0';
                }
            }
            return *this;
        }

        ~RetoMIString() {
            delete[] data;
        }

        [[nodiscard]] const char* c_str() const {
            return data;
        }

        [[nodiscard]] unsigned long size() const {
            return length;
        }
    private:
        unsigned long length;
        char *data;
    };
}

#endif // RETO_MI_STRING_INCLUDE_H