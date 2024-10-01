#include "RetoMIString.h"

namespace Retoccilus::Engine3D::ModelImporter {
    RetoMIString::RetoMIString() noexcept : _length(0), data{'\0'} { }

    RetoMIString::RetoMIString(const Retoccilus::Engine3D::ModelImporter::RetoMIString &rms)
        : _length(rms._length), data{'\0'}
    {
        if (_length > MAX_SIZE - 1) {
            _length = MAX_SIZE - 1; // Crop the string to fit the buffer size.
        }

#ifdef _MSC_VER
        ::memcpy_s(data, sizeof(data), rms.data, _length);
#else
        ::memcpy(data, rms.data, _length);
#endif

        // Add an ending tag `\0` to keep the string is a valid c-string.
        data[_length] = '\0';
    }

    RetoMIString::RetoMIString(const std::string &str) : _length((uint32_t)str.length()), data{'\0'}
    {
        if (_length > MAX_SIZE - 1) {
            _length = MAX_SIZE - 1; // Crop the string to fit the buffer size.
        }

#ifdef _MSC_VER
        ::memcpy_s(data, sizeof(data), str.c_str(), _length);
#else
        ::memcpy(data, str.c_str(), _length);
#endif
        // Add an ending tag `\0` to keep the string is a valid c-string.
        data[_length] = '\0';
    }
}