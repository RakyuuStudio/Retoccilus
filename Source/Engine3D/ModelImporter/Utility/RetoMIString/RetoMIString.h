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
    private:
        unsigned int _length;
        char data[MAX_SIZE]; // Max String Size
    };
}

#endif // RETO_MI_STRING_INCLUDE_H