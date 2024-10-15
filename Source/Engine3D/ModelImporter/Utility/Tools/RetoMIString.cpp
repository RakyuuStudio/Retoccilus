#include "RetoMIString.h"

// TODO: All the `memcpy_s` may lead to wrong result or runtime error.
//  Me from the future (or someone from the contributor team) should pay more attention on this.

namespace Retoccilus::Engine3D::ModelImporter {
    /**
     * @brief Default Constructor
     */
    RetoMIString::RetoMIString() noexcept : _length(0) {
        data[0] = '\0';
    }

    /**
     * @brief Copied Constructor that accepts a ``RetoMIString`` instance.
     * @param rms - The parameter that you need to copy
     */
    RetoMIString::RetoMIString(const Retoccilus::Engine3D::ModelImporter::RetoMIString &rms)
        : _length(rms._length) {
        if (_length > MAX_SIZE - 1) {
            _length = MAX_SIZE - 1; // Crop the string to fit the buffer size.
        }

#ifdef _MSC_VER
        ::memcpy_s(data, sizeof(data), rms.data, _length);
#else
        ::memcpy(data, rms.data, _length);
#endif
        data[_length] = '\0'; // Add an ending tag `\0` to keep the string is a valid c-string.
    }

    /**
     * @brief Constructor that accept a ``std::string`` instance.
     * @param str - The parameter that you need to copy
     */
    RetoMIString::RetoMIString(const std::string &str)
        : _length(static_cast<std::uint32_t>(str.length())) {
        if (_length > MAX_SIZE - 1) {
            _length = MAX_SIZE - 1; // Crop the string to fit the buffer size.
        }

#ifdef _MSC_VER
        ::memcpy_s(data, sizeof(data), str.c_str(), _length);
#else
        ::memcpy(data, str.c_str(), _length);
#endif
        data[_length] = '\0'; // Add an ending tag `\0` to keep the string is a valid c-string.
    }

    void RetoMIString::setData(const std::string &str) {
        _length = static_cast<std::uint32_t>(str.length());
        if (_length > MAX_SIZE - 1) {
            _length = MAX_SIZE - 1; // Crop the string to fit the buffer size.
        }

#ifdef _MSC_VER
        ::memcpy_s(data, sizeof(data), str.c_str(), _length);
#else
        ::memcpy(data, str.c_str(), _length);
#endif
        data[_length] = '\0'; // Add an ending tag `\0` to keep the string is a valid c-string.
    }

    void RetoMIString::setData(const char *charPointerArray) {
        auto lth = static_cast<std::uint32_t>(::strlen(charPointerArray));
        if (lth > MAX_SIZE - 1) {
            lth = MAX_SIZE - 1;
        }

        _length = lth;
#ifdef _MSC_VER
        ::memcpy_s(data, sizeof(data), charPointerArray, lth);
#else
        ::memcpy(data, charPointerArray, lth);
#endif
        data[lth] = '\0'; // Add an ending tag `\0` to keep the string is a valid c-string.
    }

    RetoMIString &
    RetoMIString::operator=(const Retoccilus::Engine3D::ModelImporter::RetoMIString &rms) {
        if (this != &rms) {
            _length = rms._length;
            if (_length > MAX_SIZE - 1) {
                _length = MAX_SIZE - 1;
            }

#ifdef _MSC_VER
            ::memcpy_s(data, sizeof(data), rms.data, _length);
#else
            ::memcpy(data, rms.data, _length);
#endif
            data[_length] = '\0'; // Add an ending tag `\0` to keep the string is a valid c-string.
        }
        return *this;
    }

    RetoMIString &RetoMIString::operator=(const char *charPointerArray) {
        setData(charPointerArray);
        return *this;
    }

    RetoMIString &RetoMIString::operator=(const std::string &str) {
        setData(str);
        return *this;
    }

    bool
    RetoMIString::operator==(const Retoccilus::Engine3D::ModelImporter::RetoMIString &rms) const {
        if (_length == rms._length) {
            return ::memcmp(data, rms.data, _length) == 0;
        }
        return false;
    }

    bool
    RetoMIString::operator!=(const Retoccilus::Engine3D::ModelImporter::RetoMIString &rms) const {
        return !(*this == rms);
    }

    void RetoMIString::append(const char *appender) {
        const auto lth = static_cast<std::uint32_t>(::strlen(appender));
        if (lth == 0) {
            return;
        }

        if (lth + _length >= MAX_SIZE) {
            return;
        }

#ifdef _MSC_VER
        ::memcpy_s(data + _length, MAX_SIZE - _length, appender, lth);
#else
        ::memcpy(data + _length, appender, lth);
#endif

        _length += lth;
        data[_length] = '\0'; // Add an ending tag `\0` to keep the string is a valid c-string.
    }

    void RetoMIString::clear() {
        _length = 0;
        data[0] = '\0';
    }

    const char *RetoMIString::c_str() const {
        return data;
    }
    const std::uint32_t RetoMIString::length() {
        return _length;
    }
    const char *RetoMIString::getData() const {
        return data;
    }
} // namespace Retoccilus::Engine3D::ModelImporter
