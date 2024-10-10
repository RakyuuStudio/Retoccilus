#ifndef RETO_MI_STRING_INCLUDE_H
#define RETO_MI_STRING_INCLUDE_H

#include <limits>
#include <string>

namespace Retoccilus::Engine3D::ModelImporter {
    static const int MAX_SIZE = 1024;

    class RetoMIString {
    public:
        explicit RetoMIString() noexcept;
        RetoMIString(const RetoMIString &rms);
        explicit RetoMIString(const std::string &str);

        void setData(const std::string &str);
        void setData(const char *charPointerArray);

        RetoMIString &operator=(const RetoMIString &rms);
        RetoMIString &operator=(const char *charPointerArray);
        RetoMIString &operator=(const std::string &str);

        bool operator==(const RetoMIString &rms) const;
        bool operator!=(const RetoMIString &rms) const;

        void append(const char *appender);
        void clear();
        [[nodiscard]] const char* c_str() const;
    private:
        std::uint32_t _length;
        char data[MAX_SIZE]; // Max String Size
    }; // End of Class: RetoMIString
}

#endif // RETO_MI_STRING_INCLUDE_H
